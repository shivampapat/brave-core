
/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_TOOLS_REDIRECT_CC_LOGGING_H_
#define BRAVE_TOOLS_REDIRECT_CC_LOGGING_H_

#include <sstream>

namespace logging {

bool IsVerbose();

class LogMessage {
 public:
  LogMessage(const char* file, int line);
  ~LogMessage();

  std::ostream& stream() { return stream_; }

 private:
  std::ostringstream stream_;
};

#define LOG(x)                                             \
  do {                                                     \
    logging::LogMessage(__FILE__, __LINE__).stream() << x; \
  } while (false)
#define VLOG(x)        \
  do {                 \
    if (IsVerbose()) { \
      LOG(x);          \
    }                  \
  } while (false)

}  // namespace logging

#endif  // BRAVE_TOOLS_REDIRECT_CC_LOGGING_H_
