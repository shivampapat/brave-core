/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_wallet/tx_service_manager_factory.h"

#include <memory>
#include <utility>

#include "brave/browser/brave_wallet/asset_ratio_service_factory.h"
#include "brave/browser/brave_wallet/brave_wallet_context_utils.h"
#include "brave/browser/brave_wallet/json_rpc_service_factory.h"
#include "brave/browser/brave_wallet/keyring_service_factory.h"
#include "brave/components/brave_wallet/browser/brave_wallet_constants.h"
#include "brave/components/brave_wallet/browser/tx_service_manager.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/storage_partition.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace brave_wallet {

// static
TxServiceManagerFactory* TxServiceManagerFactory::GetInstance() {
  return base::Singleton<TxServiceManagerFactory>::get();
}

// static
mojo::PendingRemote<mojom::TxServiceManager>
TxServiceManagerFactory::GetForContext(content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return mojo::PendingRemote<mojom::TxServiceManager>();
  }

  return static_cast<TxServiceManager*>(
             GetInstance()->GetServiceForBrowserContext(context, true))
      ->MakeRemote();
}

// static
mojo::PendingRemote<mojom::EthTxServiceProxy>
TxServiceManagerFactory::GetEthTxServiceProxyForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return mojo::PendingRemote<mojom::EthTxServiceProxy>();
  }

  return static_cast<TxServiceManager*>(
             GetInstance()->GetServiceForBrowserContext(context, true))
      ->MakeEthTxServiceProxyRemote();
}

// static
TxServiceManager* TxServiceManagerFactory::GetServiceForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return nullptr;
  }
  return static_cast<TxServiceManager*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
void TxServiceManagerFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::TxServiceManager> receiver) {
  auto* tx_service_manager =
      TxServiceManagerFactory::GetServiceForContext(context);
  if (tx_service_manager) {
    tx_service_manager->Bind(std::move(receiver));
  }
}

// static
void TxServiceManagerFactory::BindEthTxServiceProxyForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::EthTxServiceProxy> receiver) {
  auto* tx_service_manager =
      TxServiceManagerFactory::GetServiceForContext(context);
  if (tx_service_manager) {
    tx_service_manager->BindEthTxServiceProxy(std::move(receiver));
  }
}

TxServiceManagerFactory::TxServiceManagerFactory()
    : BrowserContextKeyedServiceFactory(
          "TxServiceManager",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(brave_wallet::JsonRpcServiceFactory::GetInstance());
  DependsOn(brave_wallet::KeyringServiceFactory::GetInstance());
  DependsOn(brave_wallet::AssetRatioServiceFactory::GetInstance());
}

TxServiceManagerFactory::~TxServiceManagerFactory() {}

KeyedService* TxServiceManagerFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new TxServiceManager(
      JsonRpcServiceFactory::GetServiceForContext(context),
      KeyringServiceFactory::GetServiceForContext(context),
      user_prefs::UserPrefs::Get(context));
}

content::BrowserContext* TxServiceManagerFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

}  // namespace brave_wallet
