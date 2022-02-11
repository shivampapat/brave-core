/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/tx_service_manager.h"

#include <utility>

#include "brave/components/brave_wallet/browser/brave_wallet_prefs.h"
#include "brave/components/brave_wallet/browser/eth_tx_service.h"
#include "brave/components/brave_wallet/browser/fil_tx_service.h"
#include "brave/components/brave_wallet/browser/solana_tx_service.h"
#include "brave/components/brave_wallet/browser/tx_service.h"

namespace brave_wallet {

namespace {

mojom::CoinType GetCoinTypeFromTxDataUnion(
    mojom::TxDataUnionPtr tx_data_union) {
  if (tx_data_union->is_solana_tx_data())
    return mojom::CoinType::SOL;

  if (tx_data_union->is_fil_tx_data())
    return mojom::CoinType::FIL;

  return mojom::CoinType::ETH;
}

}  // namespace

TxServiceManager::TxServiceManager(JsonRpcService* json_rpc_service,
                                   KeyringService* keyring_service,
                                   PrefService* prefs)
    : prefs_(prefs), weak_factory_(this) {
  tx_service_map_[mojom::CoinType::ETH] = std::unique_ptr<TxService>(
      new EthTxService(this, json_rpc_service, keyring_service, prefs));
  tx_service_map_[mojom::CoinType::SOL] = std::unique_ptr<TxService>(
      new SolanaTxService(this, json_rpc_service, keyring_service, prefs));
  tx_service_map_[mojom::CoinType::FIL] = std::unique_ptr<TxService>(
      new FilTxService(this, json_rpc_service, keyring_service, prefs));
}

TxServiceManager::~TxServiceManager() = default;

TxService* TxServiceManager::GetTxService(mojom::CoinType coin_type) {
  auto* service = tx_service_map_[coin_type].get();
  DCHECK(service);
  return service;
}

EthTxService* TxServiceManager::GetEthTxService() {
  return reinterpret_cast<EthTxService*>(GetTxService(mojom::CoinType::ETH));
}

mojo::PendingRemote<mojom::TxServiceManager> TxServiceManager::MakeRemote() {
  mojo::PendingRemote<mojom::TxServiceManager> remote;
  tx_service_manager_receivers_.Add(this,
                                    remote.InitWithNewPipeAndPassReceiver());
  return remote;
}

void TxServiceManager::Bind(
    mojo::PendingReceiver<mojom::TxServiceManager> receiver) {
  tx_service_manager_receivers_.Add(this, std::move(receiver));
}

mojo::PendingRemote<mojom::EthTxServiceProxy>
TxServiceManager::MakeEthTxServiceProxyRemote() {
  mojo::PendingRemote<mojom::EthTxServiceProxy> remote;
  eth_tx_service_receivers_.Add(this, remote.InitWithNewPipeAndPassReceiver());
  return remote;
}

void TxServiceManager::BindEthTxServiceProxy(
    mojo::PendingReceiver<mojom::EthTxServiceProxy> receiver) {
  eth_tx_service_receivers_.Add(this, std::move(receiver));
}

void TxServiceManager::AddUnapprovedTransaction(
    mojom::TxDataUnionPtr tx_data_union,
    const std::string& from,
    AddUnapprovedTransactionCallback callback) {
  auto coin_type = GetCoinTypeFromTxDataUnion(tx_data_union->Clone());
  GetTxService(coin_type)->AddUnapprovedTransaction(std::move(tx_data_union),
                                                    from, std::move(callback));
}

void TxServiceManager::ApproveTransaction(mojom::CoinType coin_type,
                                          const std::string& tx_meta_id,
                                          ApproveTransactionCallback callback) {
  GetTxService(coin_type)->ApproveTransaction(tx_meta_id, std::move(callback));
}

void TxServiceManager::RejectTransaction(mojom::CoinType coin_type,
                                         const std::string& tx_meta_id,
                                         RejectTransactionCallback callback) {
  GetTxService(coin_type)->RejectTransaction(tx_meta_id, std::move(callback));
}

void TxServiceManager::GetAllTransactionInfo(
    mojom::CoinType coin_type,
    const std::string& from,
    GetAllTransactionInfoCallback callback) {
  GetTxService(coin_type)->GetAllTransactionInfo(from, std::move(callback));
}

void TxServiceManager::SpeedupOrCancelTransaction(
    mojom::CoinType coin_type,
    const std::string& tx_meta_id,
    bool cancel,
    SpeedupOrCancelTransactionCallback callback) {
  GetTxService(coin_type)->SpeedupOrCancelTransaction(tx_meta_id, cancel,
                                                      std::move(callback));
}

void TxServiceManager::RetryTransaction(mojom::CoinType coin_type,
                                        const std::string& tx_meta_id,
                                        RetryTransactionCallback callback) {
  GetTxService(coin_type)->RetryTransaction(tx_meta_id, std::move(callback));
}

void TxServiceManager::GetTransactionMessageToSign(
    mojom::CoinType coin_type,
    const std::string& tx_meta_id,
    GetTransactionMessageToSignCallback callback) {
  GetTxService(coin_type)->GetTransactionMessageToSign(tx_meta_id,
                                                       std::move(callback));
}

void TxServiceManager::AddObserver(
    ::mojo::PendingRemote<mojom::TxServiceManagerObserver> observer) {
  observers_.Add(std::move(observer));
}

void TxServiceManager::OnTransactionStatusChanged(
    mojom::TransactionInfoPtr tx_info) {
  for (const auto& observer : observers_)
    observer->OnTransactionStatusChanged(tx_info->Clone());
}

void TxServiceManager::OnNewUnapprovedTx(mojom::TransactionInfoPtr tx_info) {
  for (const auto& observer : observers_)
    observer->OnNewUnapprovedTx(tx_info->Clone());
}

void TxServiceManager::OnUnapprovedTxUpdated(
    mojom::TransactionInfoPtr tx_info) {
  for (const auto& observer : observers_)
    observer->OnUnapprovedTxUpdated(tx_info->Clone());
}

void TxServiceManager::Reset() {
  ClearTxServiceManagerProfilePrefs(prefs_);
  for (auto const& service : tx_service_map_)
    service.second->Reset();
}

void TxServiceManager::MakeERC20TransferData(
    const std::string& to_address,
    const std::string& amount,
    MakeERC20TransferDataCallback callback) {
  GetEthTxService()->MakeERC20TransferData(to_address, amount,
                                           std::move(callback));
}

void TxServiceManager::MakeERC20ApproveData(
    const std::string& spender_address,
    const std::string& amount,
    MakeERC20ApproveDataCallback callback) {
  GetEthTxService()->MakeERC20ApproveData(spender_address, amount,
                                          std::move(callback));
}

void TxServiceManager::MakeERC721TransferFromData(
    const std::string& from,
    const std::string& to,
    const std::string& token_id,
    const std::string& contract_address,
    MakeERC721TransferFromDataCallback callback) {
  GetEthTxService()->MakeERC721TransferFromData(
      from, to, token_id, contract_address, std::move(callback));
}

void TxServiceManager::SetGasPriceAndLimitForUnapprovedTransaction(
    const std::string& tx_meta_id,
    const std::string& gas_price,
    const std::string& gas_limit,
    SetGasPriceAndLimitForUnapprovedTransactionCallback callback) {
  GetEthTxService()->SetGasPriceAndLimitForUnapprovedTransaction(
      tx_meta_id, gas_price, gas_limit, std::move(callback));
}

void TxServiceManager::SetGasFeeAndLimitForUnapprovedTransaction(
    const std::string& tx_meta_id,
    const std::string& max_priority_fee_per_gas,
    const std::string& max_fee_per_gas,
    const std::string& gas_limit,
    SetGasFeeAndLimitForUnapprovedTransactionCallback callback) {
  GetEthTxService()->SetGasFeeAndLimitForUnapprovedTransaction(
      tx_meta_id, max_priority_fee_per_gas, max_fee_per_gas, gas_limit,
      std::move(callback));
}

void TxServiceManager::SetDataForUnapprovedTransaction(
    const std::string& tx_meta_id,
    const std::vector<uint8_t>& data,
    SetDataForUnapprovedTransactionCallback callback) {
  GetEthTxService()->SetDataForUnapprovedTransaction(tx_meta_id, data,
                                                     std::move(callback));
}

void TxServiceManager::SetNonceForUnapprovedTransaction(
    const std::string& tx_meta_id,
    const std::string& nonce,
    SetNonceForUnapprovedTransactionCallback callback) {
  GetEthTxService()->SetNonceForUnapprovedTransaction(tx_meta_id, nonce,
                                                      std::move(callback));
}

void TxServiceManager::GetNonceForHardwareTransaction(
    const std::string& tx_meta_id,
    GetNonceForHardwareTransactionCallback callback) {
  GetEthTxService()->GetNonceForHardwareTransaction(tx_meta_id,
                                                    std::move(callback));
}

void TxServiceManager::ProcessHardwareSignature(
    const std::string& tx_meta_id,
    const std::string& v,
    const std::string& r,
    const std::string& s,
    ProcessHardwareSignatureCallback callback) {
  GetEthTxService()->ProcessHardwareSignature(tx_meta_id, v, r, s,
                                              std::move(callback));
}

void TxServiceManager::GetGasEstimation1559(
    GetGasEstimation1559Callback callback) {
  GetEthTxService()->GetGasEstimation1559(std::move(callback));
}

}  // namespace brave_wallet
