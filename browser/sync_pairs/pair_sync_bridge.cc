/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/sync_pairs/pair_sync_bridge.h"

#include <set>
#include <utility>
#include <vector>

#include "base/big_endian.h"
#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/check_op.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "build/build_config.h"
#include "components/sync/model/entity_change.h"
#include "components/sync/model/metadata_batch.h"
#include "components/sync/model/mutable_data_batch.h"
#include "components/sync/protocol/pair_specifics.pb.h"

namespace {
std::string GetStorageKeyFromSpecifics(
    const sync_pb::PairSpecifics& specifics) {
  return std::to_string(specifics.key());
}

std::unique_ptr<syncer::EntityData> ToEntityData(
    sync_pb::PairSpecifics specifics) {
  auto entity_data = std::make_unique<syncer::EntityData>();
  entity_data->name = base::NumberToString(specifics.key());
  entity_data->id = base::NumberToString(specifics.key());
  entity_data->specifics.mutable_pair()->Swap(&specifics);
  return entity_data;
}
}  // namespace

PairSyncBridge::PairSyncBridge(
    std::unique_ptr<syncer::ModelTypeChangeProcessor> change_processor,
    syncer::OnceModelTypeStoreFactory store_factory)
    : syncer::ModelTypeSyncBridge(std::move(change_processor)) {
  std::move(store_factory)
      .Run(syncer::PAIRS, base::BindOnce(&PairSyncBridge::OnStoreCreated,
                                         weak_ptr_factory_.GetWeakPtr()));
}

PairSyncBridge::~PairSyncBridge() {}

void PairSyncBridge::AddPair(sync_pb::PairSpecifics specifics) {
  if (!store_) {
    return;
  }

  if (!change_processor()->IsTrackingMetadata()) {
    return;
  }

  std::string storage_key = GetStorageKeyFromSpecifics(specifics);

  std::unique_ptr<syncer::ModelTypeStore::WriteBatch> write_batch =
      store_->CreateWriteBatch();
  write_batch->WriteData(storage_key, specifics.SerializeAsString());

  change_processor()->Put(storage_key, ToEntityData(std::move(specifics)),
                          write_batch->GetMetadataChangeList());

  store_->CommitWriteBatch(std::move(write_batch),
                           base::BindOnce(&PairSyncBridge::OnCommit,
                                          weak_ptr_factory_.GetWeakPtr()));

  LOG(INFO) << "PairSyncBridge::OnStoreCreated";
}

base::WeakPtr<syncer::ModelTypeControllerDelegate>
PairSyncBridge::GetControllerDelegate() {
  return change_processor()->GetControllerDelegate();
}

std::unique_ptr<syncer::MetadataChangeList>
PairSyncBridge::CreateMetadataChangeList() {
  return syncer::ModelTypeStore::WriteBatch::CreateMetadataChangeList();
}

absl::optional<syncer::ModelError> PairSyncBridge::MergeSyncData(
    std::unique_ptr<syncer::MetadataChangeList> metadata_change_list,
    syncer::EntityChangeList entity_data) {
  // DCHECK(entity_data.empty());
  DCHECK(change_processor()->IsTrackingMetadata());
  DCHECK(!change_processor()->TrackedAccountId().empty());
  return ApplySyncChanges(std::move(metadata_change_list),
                          std::move(entity_data));
}

absl::optional<syncer::ModelError> PairSyncBridge::ApplySyncChanges(
    std::unique_ptr<syncer::MetadataChangeList> metadata_change_list,
    syncer::EntityChangeList entity_changes) {
  std::unique_ptr<syncer::ModelTypeStore::WriteBatch> write_batch =
      store_->CreateWriteBatch();
  for (const std::unique_ptr<syncer::EntityChange>& change : entity_changes) {
    if (change->type() == syncer::EntityChange::ACTION_DELETE) {
      write_batch->DeleteData(change->storage_key());
    } else {
      write_batch->WriteData(
          change->storage_key(),
          change->data().specifics.pair().SerializeAsString());
    }
  }

  write_batch->TakeMetadataChangesFrom(std::move(metadata_change_list));
  store_->CommitWriteBatch(std::move(write_batch),
                           base::BindOnce(&PairSyncBridge::OnCommit,
                                          weak_ptr_factory_.GetWeakPtr()));
  return {};
}

void PairSyncBridge::GetData(StorageKeyList storage_keys,
                             DataCallback callback) {
  store_->ReadData(storage_keys, base::BindOnce(&PairSyncBridge::OnReadData,
                                                weak_ptr_factory_.GetWeakPtr(),
                                                std::move(callback)));
}

void PairSyncBridge::GetAllDataForDebugging(DataCallback callback) {
  store_->ReadAllData(base::BindOnce(&PairSyncBridge::OnReadAllData,
                                     weak_ptr_factory_.GetWeakPtr(),
                                     std::move(callback)));
}

std::string PairSyncBridge::GetClientTag(
    const syncer::EntityData& entity_data) {
  return GetStorageKey(entity_data);
}

std::string PairSyncBridge::GetStorageKey(
    const syncer::EntityData& entity_data) {
  return GetStorageKeyFromSpecifics(entity_data.specifics.pair());
}

void PairSyncBridge::ApplyStopSyncChanges(
    std::unique_ptr<syncer::MetadataChangeList> delete_metadata_change_list) {
  if (delete_metadata_change_list) {
    store_->DeleteAllDataAndMetadata(base::BindOnce(
        &PairSyncBridge::OnCommit, weak_ptr_factory_.GetWeakPtr()));
  }
}

void PairSyncBridge::OnStoreCreated(
    const absl::optional<syncer::ModelError>& error,
    std::unique_ptr<syncer::ModelTypeStore> store) {
  if (error) {
    return change_processor()->ReportError(*error);
  }

  store_ = std::move(store);
  store_->ReadAllMetadata(base::BindOnce(&PairSyncBridge::OnReadAllMetadata,
                                         weak_ptr_factory_.GetWeakPtr()));
}

void PairSyncBridge::OnReadAllMetadata(
    const absl::optional<syncer::ModelError>& error,
    std::unique_ptr<syncer::MetadataBatch> metadata_batch) {
  if (error) {
    change_processor()->ReportError(*error);
  } else {
    change_processor()->ModelReadyToSync(std::move(metadata_batch));
  }
}

void PairSyncBridge::OnReadData(
    DataCallback callback,
    const absl::optional<syncer::ModelError>& error,
    std::unique_ptr<syncer::ModelTypeStore::RecordList> data_records,
    std::unique_ptr<syncer::ModelTypeStore::IdList> missing_id_list) {
  OnReadAllData(std::move(callback), error, std::move(data_records));
}

void PairSyncBridge::OnReadAllData(
    DataCallback callback,
    const absl::optional<syncer::ModelError>& error,
    std::unique_ptr<syncer::ModelTypeStore::RecordList> data_records) {
  if (error) {
    return change_processor()->ReportError(*error);
  }

  auto batch = std::make_unique<syncer::MutableDataBatch>();
  for (const syncer::ModelTypeStore::Record& r : *data_records) {
    sync_pb::PairSpecifics specifics;

    if (specifics.ParseFromString(r.value)) {
      DCHECK_EQ(r.id, GetStorageKeyFromSpecifics(specifics));
      batch->Put(r.id, ToEntityData(std::move(specifics)));
    } else {
      return change_processor()->ReportError(
          {FROM_HERE, "Failed deserializing pairs."});
    }
  }

  std::move(callback).Run(std::move(batch));
}

void PairSyncBridge::OnCommit(const absl::optional<syncer::ModelError>& error) {
  if (error) {
    change_processor()->ReportError(*error);
  }
}
