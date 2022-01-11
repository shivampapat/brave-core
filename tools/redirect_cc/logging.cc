/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/tools/redirect_cc/logging.h"

#include <iostream>

#include "brave/tools/redirect_cc/os_utils.h"

namespace logging {

bool IsVerbose() {
  static int is_verbose = -1;
  if (is_verbose == -1) {
    FilePathString verbose_var;
    if (os_utils::GetEnvVar(FILE_PATH_LITERAL("REDIRECT_CC_VERBOSE"),
                            &verbose_var)) {
      is_verbose = verbose_var == FILE_PATH_LITERAL("1") ? 1 : 0;
    } else {
      is_verbose = 0;
    }
  }
  return is_verbose == 1;
}

LogMessage::LogMessage(const char* file, int line) {
  stream() << "[" << file << "(" << line << ")] ";
}

LogMessage::~LogMessage() {
  std::cout << stream_.str();
}

}  // namespace utils
