// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BRAVE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_BRAVE_NEW_TAB_PAGE_HANDLER_H_
#define BRAVE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_BRAVE_NEW_TAB_PAGE_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "brave/components/brave_new_tab_ui/brave_new_tab_page.mojom.h"
#include "chrome/browser/search/background/ntp_custom_background_service.h"
#include "chrome/browser/search/background/ntp_custom_background_service_observer.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace base {
class FilePath;
}  // namespace base

namespace content {
class WebContents;
}  // namespace content

namespace gfx {
class Image;
}  // namespace gfx

namespace image_fetcher {
class ImageDecoder;
}  // namespace image_fetcher

class NtpCustomBackgroundService;
class Profile;

class BraveNewTabPageHandler : public brave_new_tab_page::mojom::PageHandler,
                               public NtpCustomBackgroundServiceObserver,
                               public ui::SelectFileDialog::Listener {
 public:
  BraveNewTabPageHandler(
      mojo::PendingReceiver<brave_new_tab_page::mojom::PageHandler>
          pending_page_handler,
      mojo::PendingRemote<brave_new_tab_page::mojom::Page> pending_page,
      Profile* profile,
      NtpCustomBackgroundService* ntp_custom_background_service,
      content::WebContents* web_contents);
  ~BraveNewTabPageHandler() override;

  BraveNewTabPageHandler(const BraveNewTabPageHandler&) = delete;
  BraveNewTabPageHandler& operator=(const BraveNewTabPageHandler&) = delete;

 private:
  // brave_new_tab_page::mojom::PageHandler overrides:
  void ChooseLocalCustomBackground() override;
  void UseBraveBackground() override;

  // NtpCustomBackgroundServiceObserver overrides:
  void OnCustomBackgroundImageUpdated() override;
  void OnNtpCustomBackgroundServiceShuttingDown() override;

  // SelectFileDialog::Listener overrides:
  void FileSelected(const base::FilePath& path,
                    int index,
                    void* params) override;
  void FileSelectionCanceled(void* params) override;

  bool IsCustomBackgroundEnabled() const;
  image_fetcher::ImageDecoder* GetImageDecoder();
  void ConvertSelectedImageFileAndSave(const base::FilePath& image_file);
  void OnGotImageFile(absl::optional<std::string> input);
  void OnImageDecoded(const gfx::Image& image);
  void OnSavedEncodedImage(bool success);
  base::FilePath GetTempConvertedImageFilePath() const;
  void CleanUp();

  mojo::Receiver<brave_new_tab_page::mojom::PageHandler> page_handler_;
  mojo::Remote<brave_new_tab_page::mojom::Page> page_;
  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<NtpCustomBackgroundService> ntp_custom_background_service_ = nullptr;
  raw_ptr<content::WebContents> web_contents_ = nullptr;
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;
  base::ScopedObservation<NtpCustomBackgroundService,
                          NtpCustomBackgroundServiceObserver>
      ntp_custom_background_service_observation_{this};
  bool delete_temporarily_converted_file_ = false;
  std::unique_ptr<image_fetcher::ImageDecoder> image_decoder_;
  base::WeakPtrFactory<BraveNewTabPageHandler> weak_factory_;
};

#endif  // BRAVE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_BRAVE_NEW_TAB_PAGE_HANDLER_H_
