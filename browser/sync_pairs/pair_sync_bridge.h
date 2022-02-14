/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_SYNC_PAIRS_PAIR_SYNC_BRIDGE_H_
#define BRAVE_BROWSER_SYNC_PAIRS_PAIR_SYNC_BRIDGE_H_

#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "components/sync/model/model_type_change_processor.h"
#include "components/sync/model/model_type_store.h"
#include "components/sync/model/model_type_sync_bridge.h"

#include "components/sync/model/model_type_controller_delegate.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace sync_pb {
class PairSpecifics;
}

class PairSyncBridge : public syncer::ModelTypeSyncBridge {
 public:
  PairSyncBridge(
      std::unique_ptr<syncer::ModelTypeChangeProcessor> change_processor,
      syncer::OnceModelTypeStoreFactory store_factory);

  PairSyncBridge(const PairSyncBridge&) = delete;
  PairSyncBridge& operator=(const PairSyncBridge&) = delete;

  ~PairSyncBridge() override;

  void AddPair(sync_pb::PairSpecifics pair);

  base::WeakPtr<syncer::ModelTypeControllerDelegate> GetControllerDelegate();

  // ModelTypeSyncBridge implementation.
  std::unique_ptr<syncer::MetadataChangeList> CreateMetadataChangeList()
      override;
  absl::optional<syncer::ModelError> MergeSyncData(
      std::unique_ptr<syncer::MetadataChangeList> metadata_change_list,
      syncer::EntityChangeList entity_data) override;
  absl::optional<syncer::ModelError> ApplySyncChanges(
      std::unique_ptr<syncer::MetadataChangeList> metadata_change_list,
      syncer::EntityChangeList entity_changes) override;
  void GetData(StorageKeyList storage_keys, DataCallback callback) override;
  void GetAllDataForDebugging(DataCallback callback) override;
  std::string GetClientTag(const syncer::EntityData& entity_data) override;
  std::string GetStorageKey(const syncer::EntityData& entity_data) override;
  void ApplyStopSyncChanges(std::unique_ptr<syncer::MetadataChangeList>
                                delete_metadata_change_list) override;

 private:
  void OnStoreCreated(const absl::optional<syncer::ModelError>& error,
                      std::unique_ptr<syncer::ModelTypeStore> store);

  void OnReadAllMetadata(const absl::optional<syncer::ModelError>& error,
                         std::unique_ptr<syncer::MetadataBatch> metadata_batch);

  void OnReadData(
      DataCallback callback,
      const absl::optional<syncer::ModelError>& error,
      std::unique_ptr<syncer::ModelTypeStore::RecordList> data_records,
      std::unique_ptr<syncer::ModelTypeStore::IdList> missing_id_list);

  void OnReadAllData(
      DataCallback callback,
      const absl::optional<syncer::ModelError>& error,
      std::unique_ptr<syncer::ModelTypeStore::RecordList> data_records);

  void OnCommit(const absl::optional<syncer::ModelError>& error);

  std::unique_ptr<syncer::ModelTypeStore> store_;
  base::WeakPtrFactory<PairSyncBridge> weak_ptr_factory_{this};
};

#endif  // BRAVE_BROWSER_SYNC_PAIRS_PAIR_SYNC_BRIDGE_H_
