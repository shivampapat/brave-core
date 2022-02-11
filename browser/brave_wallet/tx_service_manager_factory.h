/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_BRAVE_WALLET_TX_SERVICE_MANAGER_FACTORY_H_
#define BRAVE_BROWSER_BRAVE_WALLET_TX_SERVICE_MANAGER_FACTORY_H_

#include "base/memory/singleton.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "content/public/browser/browser_context.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"

namespace brave_wallet {

class TxServiceManager;

class TxServiceManagerFactory : public BrowserContextKeyedServiceFactory {
 public:
  TxServiceManagerFactory(const TxServiceManagerFactory&) = delete;
  TxServiceManagerFactory& operator=(const TxServiceManagerFactory&) = delete;

  static mojo::PendingRemote<mojom::TxServiceManager> GetForContext(
      content::BrowserContext* context);
  static mojo::PendingRemote<mojom::EthTxServiceProxy>
  GetEthTxServiceProxyForContext(content::BrowserContext* context);
  static TxServiceManager* GetServiceForContext(
      content::BrowserContext* context);
  static TxServiceManagerFactory* GetInstance();
  static void BindForContext(
      content::BrowserContext* context,
      mojo::PendingReceiver<mojom::TxServiceManager> receiver);
  static void BindEthTxServiceProxyForContext(
      content::BrowserContext* context,
      mojo::PendingReceiver<mojom::EthTxServiceProxy> receiver);

 private:
  friend struct base::DefaultSingletonTraits<TxServiceManagerFactory>;

  TxServiceManagerFactory();
  ~TxServiceManagerFactory() override;

  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
};

}  // namespace brave_wallet

#endif  // BRAVE_BROWSER_BRAVE_WALLET_TX_SERVICE_MANAGER_FACTORY_H_
