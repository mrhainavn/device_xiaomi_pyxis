/*
 * Copyright (C) 2019-2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android-base/logging.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include "property_service.h"
#include "vendor_init.h"

using android::init::property_set;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[],
    char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void load_pyxisglobal() {
    property_override_dual("ro.product.model", "ro.product.vendor.model", "Mi 9 Lite");
    property_override("ro.build.description", "pyxis-user 10 QKQ1.190828.002 V11.0.1.0.QFCMIXM release-keys");
}

void load_pyxis() {
    property_override_dual("ro.product.model", "ro.product.vendor.model", "MI CC 9");
    property_override("ro.build.description", "pyxis-user 10 QKQ1.190828.002 V11.0.3.0.QFCCNXM release-keys");
}

void vendor_load_properties() {
    std::string region = android::base::GetProperty("ro.boot.hwc", "");

    if (region.find("CN") != std::string::npos) {
        load_pyxis();
    } else if (region.find("GLOBAL") != std::string::npos) {
        load_pyxisglobal();
    } else {
        LOG(ERROR) << __func__ << ": unexcepted region!";
    }
    
    property_override("ro.oem_unlock_supported", "0");
    property_override("ro.apex.updatable", "true");
    property_override("ro.control_privapp_permissions", "log");
    property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "google/coral/coral:10/QQ2A.200501.001.B2/6352890:user/release-keys");
}
