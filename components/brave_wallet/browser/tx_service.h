/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_TX_SERVICE_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_TX_SERVICE_H_

#include <string>

#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"

class PrefService;

namespace brave_wallet {

class TxServiceManager;
class JsonRpcService;
class KeyringService;

class TxService {
 public:
  TxService(TxServiceManager* tx_service_manager,
            JsonRpcService* json_rpc_service,
            KeyringService* keyring_service,
            PrefService* prefs);
  virtual ~TxService() = default;

  using AddUnapprovedTransactionCallback =
      mojom::TxServiceManager::AddUnapprovedTransactionCallback;
  using ApproveTransactionCallback =
      mojom::TxServiceManager::ApproveTransactionCallback;
  using RejectTransactionCallback =
      mojom::TxServiceManager::RejectTransactionCallback;
  using GetAllTransactionInfoCallback =
      mojom::TxServiceManager::GetAllTransactionInfoCallback;
  using SpeedupOrCancelTransactionCallback =
      mojom::TxServiceManager::SpeedupOrCancelTransactionCallback;
  using RetryTransactionCallback =
      mojom::TxServiceManager::RetryTransactionCallback;
  using GetTransactionMessageToSignCallback =
      mojom::TxServiceManager::GetTransactionMessageToSignCallback;

  virtual void AddUnapprovedTransaction(mojom::TxDataUnionPtr tx_data_union,
                                        const std::string& from,
                                        AddUnapprovedTransactionCallback) = 0;
  virtual void ApproveTransaction(const std::string& tx_meta_id,
                                  ApproveTransactionCallback) = 0;
  virtual void RejectTransaction(const std::string& tx_meta_id,
                                 RejectTransactionCallback) = 0;
  virtual void GetAllTransactionInfo(const std::string& from,
                                     GetAllTransactionInfoCallback) = 0;

  virtual void SpeedupOrCancelTransaction(
      const std::string& tx_meta_id,
      bool cancel,
      SpeedupOrCancelTransactionCallback callback) = 0;
  virtual void RetryTransaction(const std::string& tx_meta_id,
                                RetryTransactionCallback callback) = 0;

  virtual void GetTransactionMessageToSign(
      const std::string& tx_meta_id,
      GetTransactionMessageToSignCallback callback) = 0;

  virtual void Reset() = 0;

 protected:
  raw_ptr<TxServiceManager> tx_service_manager_ = nullptr;  // NOT OWNED
  raw_ptr<JsonRpcService> json_rpc_service_ = nullptr;      // NOT OWNED
  raw_ptr<KeyringService> keyring_service_ = nullptr;       // NOT OWNED
  raw_ptr<PrefService> prefs_ = nullptr;                    // NOT OWNED
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_TX_SERVICE_H_
