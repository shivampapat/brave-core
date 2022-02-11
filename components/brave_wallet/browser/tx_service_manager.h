/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_TX_SERVICE_MANAGER_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_TX_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"
#include "components/keyed_service/core/keyed_service.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote_set.h"

class PrefService;

namespace brave_wallet {

class JsonRpcService;
class KeyringService;
class TxService;
class EthTxService;

class TxServiceManager : public KeyedService,
                         public mojom::TxServiceManager,
                         public mojom::EthTxServiceProxy {
 public:
  TxServiceManager(JsonRpcService* json_rpc_service,
                   KeyringService* keyring_service,
                   PrefService* prefs);
  ~TxServiceManager() override;
  TxServiceManager(const TxServiceManager&) = delete;
  TxServiceManager operator=(const TxServiceManager&) = delete;

  mojo::PendingRemote<mojom::TxServiceManager> MakeRemote();
  void Bind(mojo::PendingReceiver<mojom::TxServiceManager> receiver);
  mojo::PendingRemote<mojom::EthTxServiceProxy> MakeEthTxServiceProxyRemote();
  void BindEthTxServiceProxy(
      mojo::PendingReceiver<mojom::EthTxServiceProxy> receiver);

  // mojom::TxServiceManager
  void AddUnapprovedTransaction(mojom::TxDataUnionPtr tx_data_union,
                                const std::string& from,
                                AddUnapprovedTransactionCallback) override;
  void ApproveTransaction(mojom::CoinType coin_type,
                          const std::string& tx_meta_id,
                          ApproveTransactionCallback) override;
  void RejectTransaction(mojom::CoinType coin_type,
                         const std::string& tx_meta_id,
                         RejectTransactionCallback) override;
  void GetAllTransactionInfo(mojom::CoinType coin_type,
                             const std::string& from,
                             GetAllTransactionInfoCallback) override;

  void SpeedupOrCancelTransaction(
      mojom::CoinType coin_type,
      const std::string& tx_meta_id,
      bool cancel,
      SpeedupOrCancelTransactionCallback callback) override;

  void RetryTransaction(mojom::CoinType coin_type,
                        const std::string& tx_meta_id,
                        RetryTransactionCallback callback) override;

  void GetTransactionMessageToSign(
      mojom::CoinType coin_type,
      const std::string& tx_meta_id,
      GetTransactionMessageToSignCallback callback) override;

  void AddObserver(
      ::mojo::PendingRemote<mojom::TxServiceManagerObserver> observer) override;

  void OnTransactionStatusChanged(mojom::TransactionInfoPtr tx_info);
  void OnNewUnapprovedTx(mojom::TransactionInfoPtr tx_info);
  void OnUnapprovedTxUpdated(mojom::TransactionInfoPtr tx_info);

  // Resets things back to the original state of TxServiceManager
  // To be used when the Wallet is reset / erased
  void Reset() override;

  // mojom::EthTxServiceProxy
  void MakeERC20TransferData(const std::string& to_address,
                             const std::string& amount,
                             MakeERC20TransferDataCallback) override;
  void MakeERC20ApproveData(const std::string& to_address,
                            const std::string& amount,
                            MakeERC20ApproveDataCallback) override;
  void MakeERC721TransferFromData(const std::string& from,
                                  const std::string& to,
                                  const std::string& token_id,
                                  const std::string& contract_address,
                                  MakeERC721TransferFromDataCallback) override;
  void SetGasPriceAndLimitForUnapprovedTransaction(
      const std::string& tx_meta_id,
      const std::string& gas_price,
      const std::string& gas_limit,
      SetGasPriceAndLimitForUnapprovedTransactionCallback callback) override;
  void SetGasFeeAndLimitForUnapprovedTransaction(
      const std::string& tx_meta_id,
      const std::string& max_priority_fee_per_gas,
      const std::string& max_fee_per_gas,
      const std::string& gas_limit,
      SetGasFeeAndLimitForUnapprovedTransactionCallback callback) override;
  void SetDataForUnapprovedTransaction(
      const std::string& tx_meta_id,
      const std::vector<uint8_t>& data,
      SetDataForUnapprovedTransactionCallback callback) override;
  void SetNonceForUnapprovedTransaction(
      const std::string& tx_meta_id,
      const std::string& nonce,
      SetNonceForUnapprovedTransactionCallback) override;
  void GetNonceForHardwareTransaction(
      const std::string& tx_meta_id,
      GetNonceForHardwareTransactionCallback callback) override;
  void ProcessHardwareSignature(
      const std::string& tx_meta_id,
      const std::string& v,
      const std::string& r,
      const std::string& s,
      ProcessHardwareSignatureCallback callback) override;
  // Gas estimation API via eth_feeHistory API
  void GetGasEstimation1559(GetGasEstimation1559Callback callback) override;

 private:
  TxService* GetTxService(mojom::CoinType coin_type);
  EthTxService* GetEthTxService();

  raw_ptr<PrefService> prefs_;  // NOT OWNED
  std::map<mojom::CoinType, std::unique_ptr<TxService>> tx_service_map_;
  mojo::RemoteSet<mojom::TxServiceManagerObserver> observers_;
  mojo::ReceiverSet<mojom::TxServiceManager> tx_service_manager_receivers_;
  mojo::ReceiverSet<mojom::EthTxServiceProxy> eth_tx_service_receivers_;

  base::WeakPtrFactory<TxServiceManager> weak_factory_;
};

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_TX_SERVICE_MANAGER_H_
