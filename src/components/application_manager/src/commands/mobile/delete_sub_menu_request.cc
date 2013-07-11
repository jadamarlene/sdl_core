/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/mobile/delete_sub_menu_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"


namespace application_manager {

namespace commands {

DeleteSubMenuRequest::DeleteSubMenuRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

DeleteSubMenuRequest::~DeleteSubMenuRequest() {
}

void DeleteSubMenuRequest::Run() {
  LOG4CXX_INFO(logger_, "DeleteSubMenuRequest::Run");

  ApplicationImpl* app =
      static_cast<ApplicationImpl*>(ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    LOG4CXX_ERROR(logger_, "Application is not registered");
    return;
  }

  if (!app->FindSubMenu(
      (*message_)[strings::msg_params][strings::menu_id].asInt()))  {
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    LOG4CXX_ERROR(logger_, "Invalid ID");
    return;
  }

  smart_objects::CSmartObject msg_params;
  msg_params[strings::msg_params][strings::menu_id] =
      (*message_)[strings::msg_params][strings::menu_id];
  msg_params[strings::msg_params][strings::app_id] =
      app->app_id();

  CreateHMIRequest(hmi_apis::FunctionID::UI_DeleteSubMenu,
                   msg_params, true);
}

}  // namespace commands

}  // namespace application_manager
