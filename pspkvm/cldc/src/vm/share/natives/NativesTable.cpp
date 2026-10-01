/* This is a generated file.  Do not modify.
 * Generated on Thu Sep 24 02:23:39 CEST 2026
 */

/*
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "jvmconfig.h"
#if !defined(ROMIZING) || !defined(PRODUCT) || \
    ENABLE_TTY_TRACE
#include "NativesTable.hpp"
#include "kni.h"


extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPixels___3B_3BIIIIIIII();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPixels___3IZIIIIIIII();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPixels___3SZIIIIIIII();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPolygon();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_fillPolygon();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_getPixels___3B_3BIIIIIII();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_getPixels___3IIIIIIII();
extern "C" void Java_com_nokia_mid_ui_DirectGraphicsImpl_getPixels___3SIIIIIII();
extern "C" jint Java_com_pspkvm_keypad_KeyMapInfo_getCurrentKeyMapForNativeControl();
extern "C" jint Java_com_pspkvm_keypad_RawState_getAnalogX();
extern "C" jint Java_com_pspkvm_keypad_RawState_getAnalogY();
extern "C" jint Java_com_pspkvm_system_Power_getBatteryLifePercent();
extern "C" jint Java_com_pspkvm_system_Power_getBatteryLifeTime();
extern "C" jint Java_com_pspkvm_system_Power_getBatteryTemp();
extern "C" jint Java_com_pspkvm_system_Power_getBatteryVolt();
extern "C" jint Java_com_pspkvm_system_Power_getBusClockFrequency();
extern "C" jint Java_com_pspkvm_system_Power_getCpuClockFrequency();
extern "C" jboolean Java_com_pspkvm_system_Power_isBatteryCharging();
extern "C" jboolean Java_com_pspkvm_system_Power_isBatteryExist();
extern "C" jboolean Java_com_pspkvm_system_Power_isLowBattery();
extern "C" jboolean Java_com_pspkvm_system_Power_isPowerOnline();
extern "C" void Java_com_pspkvm_system_VMSettings_commit();
extern "C" jobject Java_com_pspkvm_system_VMSettings_get();
extern "C" void Java_com_pspkvm_system_VMSettings_set();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getBSSID();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getChannel();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getEAPType();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getGateway();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getIP();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getMACAddr();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getPowerSave();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getPrimaryDNS();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getProfileName();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getProxyPort();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getProxyURL();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getSSID();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getSecondaryDNS();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getSecurityType();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getSignalStrength();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getStartBrowser();
extern "C" jobject Java_com_pspkvm_system_WifiStatus_getSubnetMask();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getUseProxy();
extern "C" jint Java_com_pspkvm_system_WifiStatus_getUseWiFiSP();
extern "C" jboolean Java_com_pspkvm_system_WifiStatus_isPowerOn();
extern "C" jboolean Java_com_pspkvm_system_WifiStatus_isSwitchOn();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_byteToChar();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_charToByte();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_getByteLength();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_getHandler();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_getMaxByteLength();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_sizeOfByteInUnicode();
extern "C" jint Java_com_sun_cldc_i18n_j2me_Conv_sizeOfUnicodeInByte();
extern "C" jint Java_com_sun_cldc_io_ResourceInputStream_bytesRemain();
extern "C" jobject Java_com_sun_cldc_io_ResourceInputStream_clone();
extern "C" jobject Java_com_sun_cldc_io_ResourceInputStream_open();
extern "C" jint Java_com_sun_cldc_io_ResourceInputStream_readByte();
extern "C" jint Java_com_sun_cldc_io_ResourceInputStream_readBytes();
extern "C" void Java_com_sun_cldc_isolate_Isolate_attachDebugger0();
extern "C" jobject Java_com_sun_cldc_isolate_Isolate_currentIsolate0();
extern "C" jint Java_com_sun_cldc_isolate_Isolate_exitCode0();
extern "C" jobject Java_com_sun_cldc_isolate_Isolate_getIsolates0();
extern "C" jint Java_com_sun_cldc_isolate_Isolate_getStatus();
extern "C" jint Java_com_sun_cldc_isolate_Isolate_id0();
extern "C" jint Java_com_sun_cldc_isolate_Isolate_isSuspended0();
extern "C" void Java_com_sun_cldc_isolate_Isolate_nativeStart();
extern "C" void Java_com_sun_cldc_isolate_Isolate_notifyStatus();
extern "C" void Java_com_sun_cldc_isolate_Isolate_registerNewIsolate();
extern "C" void Java_com_sun_cldc_isolate_Isolate_resume0();
extern "C" void Java_com_sun_cldc_isolate_Isolate_setPriority0();
extern "C" void Java_com_sun_cldc_isolate_Isolate_setProfile();
extern "C" void Java_com_sun_cldc_isolate_Isolate_stop();
extern "C" void Java_com_sun_cldc_isolate_Isolate_suspend0();
extern "C" jint Java_com_sun_cldc_isolate_Isolate_usedMemory0();
extern "C" void Java_com_sun_cldc_isolate_Isolate_waitStatus();
extern "C" void Java_com_sun_cldc_util_SemaphoreLock_acquire();
extern "C" void Java_com_sun_cldc_util_SemaphoreLock_release();
extern "C" void Java_com_sun_cldchi_io_ConsoleOutputStream_write();
extern "C" void Java_com_sun_cldchi_jvm_FileDescriptor_finalize();
extern "C" void Java_com_sun_cldchi_jvm_JVM_cancelImageCreation();
extern "C" jboolean Java_com_sun_cldchi_jvm_JVM_createAppImage0();
extern "C" void Java_com_sun_cldchi_jvm_JVM_createSysImage();
extern "C" jint Java_com_sun_cldchi_jvm_JVM_getAppImageProgress();
extern "C" void Java_com_sun_cldchi_jvm_JVM_loadLibrary();
extern "C" void Java_com_sun_cldchi_jvm_JVM_setLogChannel();
extern "C" void Java_com_sun_cldchi_jvm_JVM_startAppImage();
extern "C" void native_jvm_unchecked_byte_arraycopy_entry();
extern "C" void native_jvm_unchecked_char_arraycopy_entry();
extern "C" void native_jvm_unchecked_int_arraycopy_entry();
extern "C" void native_jvm_unchecked_long_arraycopy_entry();
extern "C" void native_jvm_unchecked_obj_arraycopy_entry();
extern "C" jint Java_com_sun_cldchi_jvm_JVM_verifyNextChunk();
extern "C" void Java_com_sun_j2me_location_LocationInfo_initNativeClass();
extern "C" void Java_com_sun_j2me_location_LocationProviderInfo_initNativeClass();
extern "C" void Java_com_sun_j2me_location_PlatformLocationProvider_finalize();
extern "C" jboolean Java_com_sun_j2me_location_PlatformLocationProvider_getCriteria();
extern "C" jboolean Java_com_sun_j2me_location_PlatformLocationProvider_getLastKnownLocationImpl();
extern "C" jboolean Java_com_sun_j2me_location_PlatformLocationProvider_getLastLocationImpl();
extern "C" jobject Java_com_sun_j2me_location_PlatformLocationProvider_getListOfLocationProviders();
extern "C" jint Java_com_sun_j2me_location_PlatformLocationProvider_getStateImpl();
extern "C" jint Java_com_sun_j2me_location_PlatformLocationProvider_open();
extern "C" jboolean Java_com_sun_j2me_location_PlatformLocationProvider_receiveNewLocationImpl();
extern "C" void Java_com_sun_j2me_location_PlatformLocationProvider_resetImpl();
extern "C" jboolean Java_com_sun_j2me_location_PlatformLocationProvider_waitForNewLocation();
extern "C" jint Java_com_sun_midp_appmanager_WifiSelector_connect();
extern "C" void Java_com_sun_midp_appmanager_WifiSelector_disconnect();
extern "C" jint Java_com_sun_midp_appmanager_WifiSelector_getConnectState();
extern "C" jobject Java_com_sun_midp_appmanager_WifiSelector_lookupWifiProfile();
extern "C" jobject Java_com_sun_midp_chameleon_input_InputModeFactory_getInputModeIds();
extern "C" void Java_com_sun_midp_chameleon_input_NativeInputMode_beginInput0();
extern "C" void Java_com_sun_midp_chameleon_input_NativeInputMode_endInput0();
extern "C" void Java_com_sun_midp_chameleon_input_NativeInputMode_finalize();
extern "C" jobject Java_com_sun_midp_chameleon_input_NativeInputMode_getCommandName();
extern "C" jobject Java_com_sun_midp_chameleon_input_NativeInputMode_getMatchList();
extern "C" jobject Java_com_sun_midp_chameleon_input_NativeInputMode_getName();
extern "C" jobject Java_com_sun_midp_chameleon_input_NativeInputMode_getNextMatch();
extern "C" jchar Java_com_sun_midp_chameleon_input_NativeInputMode_getPendingChar();
extern "C" jboolean Java_com_sun_midp_chameleon_input_NativeInputMode_hasMoreMatches();
extern "C" jint Java_com_sun_midp_chameleon_input_NativeInputMode_initialize();
extern "C" jobject Java_com_sun_midp_chameleon_input_NativeInputMode_processKey0();
extern "C" jboolean Java_com_sun_midp_chameleon_input_NativeInputMode_supportsConstraints();
extern "C" void Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_beginReadingSkinFile();
extern "C" void Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_finalize();
extern "C" jint Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_finishReadingSkinFile();
extern "C" jobject Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_readByteArray();
extern "C" jobject Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_readIntArray();
extern "C" jobject Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_readStringArray();
extern "C" void Java_com_sun_midp_chameleon_skins_resources_LoadedSkinResources_finalize();
extern "C" jint Java_com_sun_midp_chameleon_skins_resources_SkinResources_getRomizedImageDataArrayLength();
extern "C" jint Java_com_sun_midp_chameleon_skins_resources_SkinResources_getRomizedImageDataArrayPtr();
extern "C" jobject Java_com_sun_midp_chameleon_skins_resources_SkinResources_getSharedResourcePool();
extern "C" jobject Java_com_sun_midp_chameleon_skins_resources_SkinResources_getSharedSkinData();
extern "C" jboolean Java_com_sun_midp_chameleon_skins_resources_SkinResources_ifLoadAllResources();
extern "C" void Java_com_sun_midp_chameleon_skins_resources_SkinResources_shareResourcePool();
extern "C" void Java_com_sun_midp_chameleon_skins_resources_SkinResources_shareSkinData();
extern "C" void Java_com_sun_midp_crypto_ARC4_nativetx();
extern "C" void Java_com_sun_midp_crypto_MD2_nativeFinal();
extern "C" void Java_com_sun_midp_crypto_MD2_nativeUpdate();
extern "C" void Java_com_sun_midp_crypto_MD5_nativeFinal();
extern "C" void Java_com_sun_midp_crypto_MD5_nativeUpdate();
extern "C" jint Java_com_sun_midp_crypto_RSA_modExp();
extern "C" void Java_com_sun_midp_crypto_SHA_nativeFinal();
extern "C" void Java_com_sun_midp_crypto_SHA_nativeUpdate();
extern "C" void Java_com_sun_midp_events_EventQueue_finalize();
extern "C" jint Java_com_sun_midp_events_EventQueue_getNativeEventQueueHandle();
extern "C" void Java_com_sun_midp_events_EventQueue_handleFatalError();
extern "C" void Java_com_sun_midp_events_EventQueue_resetNativeEventQueue();
extern "C" void Java_com_sun_midp_events_EventQueue_sendNativeEventToIsolate();
extern "C" void Java_com_sun_midp_events_EventQueue_sendShutdownEvent();
extern "C" jboolean Java_com_sun_midp_events_NativeEventMonitor_readNativeEvent();
extern "C" jint Java_com_sun_midp_events_NativeEventMonitor_waitForNativeEvent();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_devIdToDispId();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_dispIdToDevId();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getCurrentDevice0();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getDefaultKeymap0();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getDeviceHeight0();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getDeviceKeyCode0();
extern "C" jobject Java_com_sun_midp_installer_DeviceDesc_getDeviceName0();
extern "C" jobject Java_com_sun_midp_installer_DeviceDesc_getDevicePropid0();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getDeviceWidth0();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getDevicesNumber0();
extern "C" jint Java_com_sun_midp_installer_DeviceDesc_getJavaKeyNumber0();
extern "C" void Java_com_sun_midp_installer_DeviceDesc_resetKeymap0();
extern "C" void Java_com_sun_midp_installer_DeviceDesc_setCurrentCPUSpeed0();
extern "C" void Java_com_sun_midp_installer_DeviceDesc_setCurrentDevice0();
extern "C" void Java_com_sun_midp_installer_DeviceDesc_setDefaultKeymap0();
extern "C" void Java_com_sun_midp_installer_DeviceDesc_setKeymap0();
extern "C" void Java_com_sun_midp_installer_OtaNotifier_addInstallNotification();
extern "C" void Java_com_sun_midp_installer_OtaNotifier_fillDeleteNotificationListForRetry();
extern "C" jboolean Java_com_sun_midp_installer_OtaNotifier_getInstallNotificationForRetry();
extern "C" jint Java_com_sun_midp_installer_OtaNotifier_getNumberOfDeleteNotifications();
extern "C" void Java_com_sun_midp_installer_OtaNotifier_removeDeleteNotification();
extern "C" void Java_com_sun_midp_installer_OtaNotifier_removeInstallNotification();
extern "C" void Java_com_sun_midp_installer_SuiteDownloadInfo_closeDir();
extern "C" jobject Java_com_sun_midp_installer_SuiteDownloadInfo_convert2lable0();
extern "C" jobject Java_com_sun_midp_installer_SuiteDownloadInfo_nextFileInDir0();
extern "C" jint Java_com_sun_midp_installer_SuiteDownloadInfo_openDir();
extern "C" void Java_com_sun_midp_io_NetworkConnectionBase_initializeInternal();
extern "C" void Java_com_sun_midp_io_j2me_comm_Protocol_finalize();
extern "C" void Java_com_sun_midp_io_j2me_comm_Protocol_native_1close();
extern "C" void Java_com_sun_midp_io_j2me_comm_Protocol_native_1configurePort();
extern "C" jint Java_com_sun_midp_io_j2me_comm_Protocol_native_1openByName();
extern "C" jint Java_com_sun_midp_io_j2me_comm_Protocol_native_1readBytes();
extern "C" jint Java_com_sun_midp_io_j2me_comm_Protocol_native_1writeBytes();
extern "C" jobject Java_com_sun_midp_io_j2me_datagram_Protocol_addrToString();
extern "C" void Java_com_sun_midp_io_j2me_datagram_Protocol_close0();
extern "C" void Java_com_sun_midp_io_j2me_datagram_Protocol_finalize();
extern "C" jobject Java_com_sun_midp_io_j2me_datagram_Protocol_getHost0();
extern "C" jint Java_com_sun_midp_io_j2me_datagram_Protocol_getIpNumber();
extern "C" jint Java_com_sun_midp_io_j2me_datagram_Protocol_getMaximumLength0();
extern "C" jint Java_com_sun_midp_io_j2me_datagram_Protocol_getNominalLength0();
extern "C" jint Java_com_sun_midp_io_j2me_datagram_Protocol_getPort0();
extern "C" void Java_com_sun_midp_io_j2me_datagram_Protocol_open0();
extern "C" jlong Java_com_sun_midp_io_j2me_datagram_Protocol_receive0();
extern "C" jint Java_com_sun_midp_io_j2me_datagram_Protocol_send0();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_availableSize();
extern "C" jboolean Java_com_sun_midp_io_j2me_file_DefaultFileHandler_canRead();
extern "C" jboolean Java_com_sun_midp_io_j2me_file_DefaultFileHandler_canWrite();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_close();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeDir();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeForRead();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeForReadWrite();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeForWrite();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_create();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_delete();
extern "C" jobject Java_com_sun_midp_io_j2me_file_DefaultFileHandler_dirGetNextFile();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_directorySize();
extern "C" jboolean Java_com_sun_midp_io_j2me_file_DefaultFileHandler_exists();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_fileSize();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_finalize();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_flush();
extern "C" jchar Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getFileSeparator();
extern "C" jobject Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getMountedRoots();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getNativeName();
extern "C" jobject Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getNativePathForRoot();
extern "C" jobject Java_com_sun_midp_io_j2me_file_DefaultFileHandler_illegalFileNameChars0();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_initialize();
extern "C" jboolean Java_com_sun_midp_io_j2me_file_DefaultFileHandler_isDirectory();
extern "C" jboolean Java_com_sun_midp_io_j2me_file_DefaultFileHandler_isHidden0();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_lastModified();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_mkdir();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_openDir();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_openForRead();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_openForWrite();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_positionForWrite();
extern "C" jint Java_com_sun_midp_io_j2me_file_DefaultFileHandler_read();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_rename0();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_setHidden0();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_setReadable();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_setWritable();
extern "C" jlong Java_com_sun_midp_io_j2me_file_DefaultFileHandler_totalSize();
extern "C" void Java_com_sun_midp_io_j2me_file_DefaultFileHandler_truncate();
extern "C" jint Java_com_sun_midp_io_j2me_file_DefaultFileHandler_write();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_add0();
extern "C" jlong Java_com_sun_midp_io_j2me_push_ConnectionRegistry_addAlarm0();
extern "C" void Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByHandle0();
extern "C" void Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByMidlet0();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByName0();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_del0();
extern "C" void Java_com_sun_midp_io_j2me_push_ConnectionRegistry_delAllForSuite0();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_getEntry0();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_getMIDlet0();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_list0();
extern "C" jint Java_com_sun_midp_io_j2me_push_ConnectionRegistry_poll0();
extern "C" jint Java_com_sun_midp_io_j2me_socket_Protocol_available0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_close0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_finalize();
extern "C" jobject Java_com_sun_midp_io_j2me_socket_Protocol_getHost0();
extern "C" jint Java_com_sun_midp_io_j2me_socket_Protocol_getIpNumber0();
extern "C" jint Java_com_sun_midp_io_j2me_socket_Protocol_getPort0();
extern "C" jint Java_com_sun_midp_io_j2me_socket_Protocol_getSockOpt0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_notifyClosedInput0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_notifyClosedOutput0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_open0();
extern "C" jint Java_com_sun_midp_io_j2me_socket_Protocol_read0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_setSockOpt0();
extern "C" void Java_com_sun_midp_io_j2me_socket_Protocol_shutdownOutput0();
extern "C" jint Java_com_sun_midp_io_j2me_socket_Protocol_write0();
extern "C" jlong Java_com_sun_midp_io_j2me_storage_File_availableStorage();
extern "C" void Java_com_sun_midp_io_j2me_storage_File_deleteStorage();
extern "C" jobject Java_com_sun_midp_io_j2me_storage_File_initConfigRoot();
extern "C" jobject Java_com_sun_midp_io_j2me_storage_File_initStorageRoot();
extern "C" void Java_com_sun_midp_io_j2me_storage_File_renameStorage();
extern "C" jboolean Java_com_sun_midp_io_j2me_storage_File_storageExists();
extern "C" void Java_com_sun_midp_io_j2me_storage_RandomAccessStream_close();
extern "C" void Java_com_sun_midp_io_j2me_storage_RandomAccessStream_commitWrite();
extern "C" void Java_com_sun_midp_io_j2me_storage_RandomAccessStream_finalize();
extern "C" jint Java_com_sun_midp_io_j2me_storage_RandomAccessStream_open();
extern "C" void Java_com_sun_midp_io_j2me_storage_RandomAccessStream_position();
extern "C" jint Java_com_sun_midp_io_j2me_storage_RandomAccessStream_read();
extern "C" jint Java_com_sun_midp_io_j2me_storage_RandomAccessStream_sizeOf();
extern "C" void Java_com_sun_midp_io_j2me_storage_RandomAccessStream_truncateStream();
extern "C" void Java_com_sun_midp_io_j2me_storage_RandomAccessStream_write();
extern "C" jobject Java_com_sun_midp_jarutil_JarReader_readJarEntry0();
extern "C" void Java_com_sun_midp_jsr075_Initializer_cleanup();
extern "C" void Java_com_sun_midp_jsr075_Initializer_init();
extern "C" jobject Java_com_sun_midp_l10n_LocalizedStringsBase_getContent();
extern "C" jobject Java_com_sun_midp_l10n_LocalizedStringsBasezhCN_getContent();
extern "C" jboolean Java_com_sun_midp_lcdui_DisplayDeviceAccess_isBacklightSupported0();
extern "C" void Java_com_sun_midp_lcdui_DisplayDeviceAccess_setDeviceScreenSize();
extern "C" jboolean Java_com_sun_midp_lcdui_DisplayDeviceAccess_toggleBacklight0();
extern "C" void Java_com_sun_midp_links_Link_close();
extern "C" void Java_com_sun_midp_links_Link_finalize();
extern "C" void Java_com_sun_midp_links_Link_init0();
extern "C" jboolean Java_com_sun_midp_links_Link_isOpen();
extern "C" void Java_com_sun_midp_links_Link_receive0();
extern "C" void Java_com_sun_midp_links_Link_send0();
extern "C" jint Java_com_sun_midp_links_LinkPortal_getLinkCount0();
extern "C" void Java_com_sun_midp_links_LinkPortal_getLinks0();
extern "C" void Java_com_sun_midp_links_LinkPortal_setLinks0();
extern "C" void Java_com_sun_midp_log_LoggingBase_report();
extern "C" jboolean Java_com_sun_midp_main_AppIsolateMIDletSuiteLoader_allocateReservedResources0();
extern "C" void Java_com_sun_midp_main_AppIsolateMIDletSuiteLoader_finalize();
extern "C" void Java_com_sun_midp_main_AppIsolateMIDletSuiteLoader_handleFatalError();
extern "C" jboolean Java_com_sun_midp_main_CldcPlatformRequest_dispatchPlatformRequest();
extern "C" void Java_com_sun_midp_main_CommandState_exitInternal();
extern "C" void Java_com_sun_midp_main_CommandState_restoreCommandState();
extern "C" void Java_com_sun_midp_main_CommandState_saveCommandState();
extern "C" jobject Java_com_sun_midp_main_Configuration_getProperty0();
extern "C" void Java_com_sun_midp_main_IndicatorManager_toggleHomeIcon0();
extern "C" jboolean Java_com_sun_midp_main_MIDletAppImageGenerator_removeAppImage();
extern "C" void Java_com_sun_midp_main_MIDletProxyList_notifyResumeAll0();
extern "C" void Java_com_sun_midp_main_MIDletProxyList_notifySuspendAll0();
extern "C" void Java_com_sun_midp_main_MIDletProxyList_setForegroundInNativeState();
extern "C" jint Java_com_sun_midp_main_MIDletSuiteUtils_getAmsIsolateId();
extern "C" jint Java_com_sun_midp_main_MIDletSuiteUtils_getIsolateId();
extern "C" jboolean Java_com_sun_midp_main_MIDletSuiteUtils_isAmsIsolate();
extern "C" void Java_com_sun_midp_main_MIDletSuiteUtils_registerAmsIsolateId();
extern "C" void Java_com_sun_midp_main_MIDletSuiteUtils_vmBeginStartUp();
extern "C" void Java_com_sun_midp_main_MIDletSuiteUtils_vmEndStartUp();
extern "C" jboolean Java_com_sun_midp_main_MIDletSuiteVerifier_checkJarHash();
extern "C" jobject Java_com_sun_midp_main_MIDletSuiteVerifier_getJarHash();
extern "C" void Java_com_sun_midp_main_MIDletSuiteVerifier_useClassVerifier();
extern "C" void Java_com_sun_midp_midletsuite_InstallInfo_load();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteImpl_finalize();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteImpl_lockMIDletSuite();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteImpl_unlockMIDletSuite();
extern "C" jint Java_com_sun_midp_midletsuite_MIDletSuiteStorage_createSuiteID();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_disable();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_enable();
extern "C" jobject Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMIDletSuiteIcon0();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMIDletSuiteInfoImpl0();
extern "C" jobject Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMidletSuiteAppImagePath();
extern "C" jobject Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMidletSuiteJarPath();
extern "C" jint Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getNumberOfSuites();
extern "C" jint Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getStorageUsed();
extern "C" jint Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getSuiteID();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getSuiteList();
extern "C" jobject Java_com_sun_midp_midletsuite_MIDletSuiteStorage_loadCachedIcon0();
extern "C" jint Java_com_sun_midp_midletsuite_MIDletSuiteStorage_loadSuitesIcons0();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_nativeStoreSuite();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_remove0();
extern "C" void Java_com_sun_midp_midletsuite_MIDletSuiteStorage_storeSuiteVerifyHash();
extern "C" jboolean Java_com_sun_midp_midletsuite_MIDletSuiteStorage_suiteExists();
extern "C" jobject Java_com_sun_midp_midletsuite_MIDletSuiteStorage_suiteIdToString();
extern "C" jobject Java_com_sun_midp_midletsuite_SuiteProperties_load();
extern "C" void Java_com_sun_midp_midletsuite_SuiteSettings_load0();
extern "C" void Java_com_sun_midp_midletsuite_SuiteSettings_save0();
extern "C" jboolean Java_com_sun_midp_rms_RecordStoreFactory_suiteHasRmsData();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_closeFile();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_commitWrite();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_finalize();
extern "C" jint Java_com_sun_midp_rms_RecordStoreFile_getNumberOfStores();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_getRecordStoreList();
extern "C" jint Java_com_sun_midp_rms_RecordStoreFile_openRecordStoreFile();
extern "C" jint Java_com_sun_midp_rms_RecordStoreFile_readBytes();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_removeRecordStores();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_setPosition();
extern "C" jint Java_com_sun_midp_rms_RecordStoreFile_spaceAvailableNewRecordStore();
extern "C" jint Java_com_sun_midp_rms_RecordStoreFile_spaceAvailableRecordStore();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_truncateFile();
extern "C" void Java_com_sun_midp_rms_RecordStoreFile_writeBytes();
extern "C" void Java_com_sun_midp_rms_RecordStoreUtil_deleteFile();
extern "C" jboolean Java_com_sun_midp_rms_RecordStoreUtil_exists();
extern "C" jboolean Java_com_sun_midp_suspend_SuspendSystem_isResumePending();
extern "C" jboolean Java_com_sun_midp_suspend_SuspendSystem_00024MIDPSystem_allMidletsKilled();
extern "C" void Java_com_sun_midp_suspend_SuspendSystem_00024MIDPSystem_suspended0();
extern "C" jobject Java_com_sun_midp_util_ResourceHandler_loadRomizedResource0();
extern "C" jboolean Java_com_sun_mmedia_DefaultConfiguration_nIsAmrSupported();
extern "C" jboolean Java_com_sun_mmedia_DefaultConfiguration_nIsJtsSupported();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetBankList();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetChannelVolume();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetKeyName();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetMaxPitch();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetMaxRate();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetMinPitch();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetMinRate();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetPitch();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetProgram();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetProgramList();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetProgramName();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetRate();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nGetTempo();
extern "C" jboolean Java_com_sun_mmedia_DirectMIDIControl_nIsBankQuerySupported();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nLongMidiEvent();
extern "C" void Java_com_sun_mmedia_DirectMIDIControl_nSetChannelVolume();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nSetPitch();
extern "C" void Java_com_sun_mmedia_DirectMIDIControl_nSetProgram();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nSetRate();
extern "C" jint Java_com_sun_mmedia_DirectMIDIControl_nSetTempo();
extern "C" void Java_com_sun_mmedia_DirectMIDIControl_nShortMidiEvent();
extern "C" void Java_com_sun_mmedia_DirectPlayer_finalize();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nAcquireDevice();
extern "C" jint Java_com_sun_mmedia_DirectPlayer_nBuffering();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nFlushBuffer();
extern "C" jint Java_com_sun_mmedia_DirectPlayer_nGetDuration();
extern "C" jint Java_com_sun_mmedia_DirectPlayer_nGetMediaTime();
extern "C" jint Java_com_sun_mmedia_DirectPlayer_nInit();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nIsNeedBuffering();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nIsSupportRecording();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nPause();
extern "C" void Java_com_sun_mmedia_DirectPlayer_nReleaseDevice();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nResume();
extern "C" jint Java_com_sun_mmedia_DirectPlayer_nSetMediaTime();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nStart();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nStop();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nSwitchToBackground();
extern "C" jboolean Java_com_sun_mmedia_DirectPlayer_nSwitchToForeground();
extern "C" jint Java_com_sun_mmedia_DirectPlayer_nTerm();
extern "C" void Java_com_sun_mmedia_DirectRecord_finalize();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nClose();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nCommit();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nGetRecordedData();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nGetRecordedSize();
extern "C" jobject Java_com_sun_mmedia_DirectRecord_nGetRecordedType();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nPause();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nReset();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nSetLocator();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nSetSizeLimit();
extern "C" jboolean Java_com_sun_mmedia_DirectRecord_nSetSizeLimitIsSupported();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nStart();
extern "C" jint Java_com_sun_mmedia_DirectRecord_nStop();
extern "C" jint Java_com_sun_mmedia_DirectVideo_nGetHeight();
extern "C" jint Java_com_sun_mmedia_DirectVideo_nGetScreenHeight();
extern "C" jint Java_com_sun_mmedia_DirectVideo_nGetScreenWidth();
extern "C" jint Java_com_sun_mmedia_DirectVideo_nGetWidth();
extern "C" jint Java_com_sun_mmedia_DirectVideo_nSetAlpha();
extern "C" jboolean Java_com_sun_mmedia_DirectVideo_nSetLocation();
extern "C" jboolean Java_com_sun_mmedia_DirectVideo_nSetVisible();
extern "C" jobject Java_com_sun_mmedia_DirectVideo_nSnapShot();
extern "C" jint Java_com_sun_mmedia_DirectVolume_nGetVolume();
extern "C" jboolean Java_com_sun_mmedia_DirectVolume_nIsMuted();
extern "C" jboolean Java_com_sun_mmedia_DirectVolume_nSetMute();
extern "C" jint Java_com_sun_mmedia_DirectVolume_nSetVolume();
extern "C" void Java_com_sun_mmedia_NativeTonePlayer_finalize();
extern "C" jboolean Java_com_sun_mmedia_NativeTonePlayer_nPlayTone();
extern "C" jboolean Java_com_sun_mmedia_NativeTonePlayer_nStopTone();
extern "C" void Java_com_sun_pisces_AbstractSurface_drawRGBImpl();
extern "C" void Java_com_sun_pisces_AbstractSurface_drawSurfaceImpl();
extern "C" jint Java_com_sun_pisces_AbstractSurface_getHeight();
extern "C" void Java_com_sun_pisces_AbstractSurface_getRGB();
extern "C" jint Java_com_sun_pisces_AbstractSurface_getWidth();
extern "C" void Java_com_sun_pisces_AbstractSurface_nativeFinalize();
extern "C" void Java_com_sun_pisces_AbstractSurface_setRGB();
extern "C" void Java_com_sun_pisces_GraphicsSurfaceDestination_drawRGBImpl();
extern "C" void Java_com_sun_pisces_GraphicsSurfaceDestination_drawSurfaceImpl();
extern "C" void Java_com_sun_pisces_GraphicsSurfaceDestination_initialize();
extern "C" void Java_com_sun_pisces_NativeFinalizer_initialize();
extern "C" void Java_com_sun_pisces_NativeFinalizer_00024RendererNativeFinalizer_finalize();
extern "C" void Java_com_sun_pisces_NativeFinalizer_00024SurfaceNativeFinalizer_finalize();
extern "C" void Java_com_sun_pisces_NativeSurface_initialize();
extern "C" void Java_com_sun_pisces_PiscesFinalizer_finalize();
extern "C" void Java_com_sun_pisces_PiscesRenderer_beginRendering__I();
extern "C" void Java_com_sun_pisces_PiscesRenderer_beginRendering__IIIII();
extern "C" void Java_com_sun_pisces_PiscesRenderer_clearRect();
extern "C" void Java_com_sun_pisces_PiscesRenderer_close();
extern "C" void Java_com_sun_pisces_PiscesRenderer_cubicTo();
extern "C" void Java_com_sun_pisces_PiscesRenderer_drawArc();
extern "C" void Java_com_sun_pisces_PiscesRenderer_drawLine();
extern "C" void Java_com_sun_pisces_PiscesRenderer_drawOval();
extern "C" void Java_com_sun_pisces_PiscesRenderer_drawRect();
extern "C" void Java_com_sun_pisces_PiscesRenderer_drawRoundRect();
extern "C" void Java_com_sun_pisces_PiscesRenderer_end();
extern "C" void Java_com_sun_pisces_PiscesRenderer_endRendering();
extern "C" void Java_com_sun_pisces_PiscesRenderer_fillArc();
extern "C" void Java_com_sun_pisces_PiscesRenderer_fillOval();
extern "C" void Java_com_sun_pisces_PiscesRenderer_fillRect();
extern "C" void Java_com_sun_pisces_PiscesRenderer_fillRoundRect();
extern "C" jboolean Java_com_sun_pisces_PiscesRenderer_getAntialiasing();
extern "C" void Java_com_sun_pisces_PiscesRenderer_getBoundingBox();
extern "C" void Java_com_sun_pisces_PiscesRenderer_getTransformImpl();
extern "C" void Java_com_sun_pisces_PiscesRenderer_initialize();
extern "C" void Java_com_sun_pisces_PiscesRenderer_lineJoin();
extern "C" void Java_com_sun_pisces_PiscesRenderer_lineTo();
extern "C" void Java_com_sun_pisces_PiscesRenderer_moveTo();
extern "C" void Java_com_sun_pisces_PiscesRenderer_nativeFinalize();
extern "C" void Java_com_sun_pisces_PiscesRenderer_quadTo();
extern "C" void Java_com_sun_pisces_PiscesRenderer_resetClip();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setAntialiasing();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setClip();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setColor();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setComposite();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setCompositeRule();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setFill();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setLinearGradientImpl();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setPathData();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setRadialGradientImpl();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setStroke__();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setStroke__IIII_3II();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setTextureImpl();
extern "C" void Java_com_sun_pisces_PiscesRenderer_setTransform();
extern "C" void Java_com_sun_pisces_PiscesRenderer_staticInitialize();
extern "C" void Java_com_sun_pisces_Transform6_initialize();
extern "C" jobject Java_java_lang_Class_forName();
extern "C" jobject Java_java_lang_Class_getName();
extern "C" jobject Java_java_lang_Class_getSuperclass();
extern "C" void Java_java_lang_Class_init9();
extern "C" void Java_java_lang_Class_invoke_1clinit();
extern "C" void Java_java_lang_Class_invoke_1verify();
extern "C" jboolean Java_java_lang_Class_isArray();
extern "C" jboolean Java_java_lang_Class_isAssignableFrom();
extern "C" jboolean Java_java_lang_Class_isInstance();
extern "C" jboolean Java_java_lang_Class_isInterface();
extern "C" jobject Java_java_lang_Class_newInstance();
extern "C" jlong Java_java_lang_Double_doubleToLongBits();
extern "C" jdouble Java_java_lang_Double_longBitsToDouble();
extern "C" jint Java_java_lang_Float_floatToIntBits();
extern "C" jfloat Java_java_lang_Float_intBitsToFloat();
extern "C" void native_integer_toString_entry();
extern "C" void native_math_ceil_entry();
extern "C" void native_math_cos_entry();
extern "C" void native_math_floor_entry();
extern "C" void native_math_sin_entry();
extern "C" void native_math_sqrt_entry();
extern "C" void native_math_tan_entry();
extern "C" jobject Java_java_lang_Object_getClass();
extern "C" jint Java_java_lang_Object_hashCode();
extern "C" void Java_java_lang_Object_notify();
extern "C" void Java_java_lang_Object_notifyAll();
extern "C" void Java_java_lang_Object_wait();
extern "C" void Java_java_lang_Runtime_exitInternal();
extern "C" jlong Java_java_lang_Runtime_freeMemory();
extern "C" void Java_java_lang_Runtime_gc();
extern "C" jlong Java_java_lang_Runtime_totalMemory();
extern "C" void native_string_init_entry();
extern "C" void native_string_charAt_entry();
extern "C" void native_string_endsWith_entry();
extern "C" void native_string_equals_entry();
extern "C" jint Java_java_lang_String_hashCode();
extern "C" void native_string_indexof0_entry();
extern "C" void native_string_indexof_entry();
extern "C" void native_string_indexof0_string_entry();
extern "C" void native_string_indexof_string_entry();
extern "C" jobject Java_java_lang_String_intern();
extern "C" jint Java_java_lang_String_lastIndexOf__I();
extern "C" jint Java_java_lang_String_lastIndexOf__II();
extern "C" void native_string_startsWith0_entry();
extern "C" void native_string_startsWith_entry();
extern "C" void native_string_substringI_entry();
extern "C" void native_string_substringII_entry();
extern "C" void native_integer_toString_entry();
extern "C" void native_stringbuffer_append_entry();
extern "C" void Java_java_lang_System_arraycopy();
extern "C" void native_system_arraycopy_entry();
extern "C" jlong Java_java_lang_System_currentTimeMillis();
extern "C" jobject Java_java_lang_System_getProperty0();
extern "C" jint Java_java_lang_System_identityHashCode();
extern "C" void Java_java_lang_System_quickNativeThrow();
extern "C" jint Java_java_lang_Thread_activeCount();
extern "C" jobject Java_java_lang_Thread_currentThread();
extern "C" void Java_java_lang_Thread_internalExit();
extern "C" void Java_java_lang_Thread_interrupt0();
extern "C" jboolean Java_java_lang_Thread_isAlive();
extern "C" void Java_java_lang_Thread_setPriority0();
extern "C" void Java_java_lang_Thread_sleep();
extern "C" void Java_java_lang_Thread_start0();
extern "C" void Java_java_lang_Thread_yield();
extern "C" void Java_java_lang_Throwable_fillInStackTrace();
extern "C" void Java_java_lang_Throwable_printStackTrace();
extern "C" void Java_java_lang_ref_WeakReference_clear();
extern "C" void Java_java_lang_ref_WeakReference_finalize();
extern "C" jobject Java_java_lang_ref_WeakReference_get();
extern "C" void Java_java_lang_ref_WeakReference_initializeWeakReference();
extern "C" void native_vector_addElement_entry();
extern "C" void native_vector_elementAt_entry();
extern "C" void Java_javax_microedition_io_file_FileSystemEventHandlerBase_finalize();
extern "C" void Java_javax_microedition_io_file_FileSystemEventHandlerBase_registerListener();
extern "C" jobject Java_javax_microedition_lcdui_Clipboard_get();
extern "C" void Java_javax_microedition_lcdui_Clipboard_set();
extern "C" void Java_javax_microedition_lcdui_Display_drawTrustedIcon0();
extern "C" void Java_javax_microedition_lcdui_Display_gainedForeground0();
extern "C" jboolean Java_javax_microedition_lcdui_Display_getReverseOrientation0();
extern "C" jint Java_javax_microedition_lcdui_Display_getScreenHeight0();
extern "C" jint Java_javax_microedition_lcdui_Display_getScreenWidth0();
extern "C" void Java_javax_microedition_lcdui_Display_refresh0();
extern "C" jboolean Java_javax_microedition_lcdui_Display_reverseOrientation0();
extern "C" void Java_javax_microedition_lcdui_Display_setFullScreen0();
extern "C" jboolean Java_javax_microedition_lcdui_Display_vibrate0();
extern "C" jint Java_javax_microedition_lcdui_Font_charWidth();
extern "C" jint Java_javax_microedition_lcdui_Font_charsWidth();
extern "C" void Java_javax_microedition_lcdui_Font_init();
extern "C" jint Java_javax_microedition_lcdui_Font_stringWidth();
extern "C" jint Java_javax_microedition_lcdui_Font_substringWidth();
extern "C" void Java_javax_microedition_lcdui_Graphics_doCopyArea();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawArc();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawChar();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawChars();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawLine();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawRGB();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawRect();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawRoundRect();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawString();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawSubstring();
extern "C" void Java_javax_microedition_lcdui_Graphics_drawUtilityString();
extern "C" void Java_javax_microedition_lcdui_Graphics_fillArc();
extern "C" void Java_javax_microedition_lcdui_Graphics_fillRect();
extern "C" void Java_javax_microedition_lcdui_Graphics_fillRoundRect();
extern "C" void Java_javax_microedition_lcdui_Graphics_fillTriangle();
extern "C" jint Java_javax_microedition_lcdui_Graphics_getDisplayColor();
extern "C" jint Java_javax_microedition_lcdui_Graphics_getPixel();
extern "C" jboolean Java_javax_microedition_lcdui_Graphics_render();
extern "C" jboolean Java_javax_microedition_lcdui_Graphics_renderRegion();
extern "C" void Java_javax_microedition_lcdui_ImageData_getRGB();
extern "C" jboolean Java_javax_microedition_lcdui_ImageDataFactory_loadCachedImage0();
extern "C" void Java_javax_microedition_lcdui_ImageDataFactory_loadJPEG();
extern "C" jboolean Java_javax_microedition_lcdui_ImageDataFactory_loadGIF();
extern "C" jboolean Java_javax_microedition_lcdui_ImageDataFactory_loadPNG();
extern "C" void Java_javax_microedition_lcdui_ImageDataFactory_loadRAW();
extern "C" void Java_javax_microedition_lcdui_ImageDataFactory_loadRGB();
extern "C" void Java_javax_microedition_lcdui_ImageDataFactory_loadRegion();
extern "C" jboolean Java_javax_microedition_lcdui_ImageDataFactory_loadRomizedImage();
extern "C" jint Java_javax_microedition_lcdui_KeyConverter_getGameAction();
extern "C" jint Java_javax_microedition_lcdui_KeyConverter_getKeyCode();
extern "C" jobject Java_javax_microedition_lcdui_KeyConverter_getKeyName();
extern "C" jint Java_javax_microedition_lcdui_KeyConverter_getSystemKey();
extern "C" jint Java_javax_microedition_lcdui_TextFieldLFImpl_launchNativeTextField0();
extern "C" void Java_javax_microedition_lcdui_game_GameCanvas_setSuppressKeyEvents();
extern "C" jdouble Java_javax_microedition_location_Coordinates_atan2();
extern "C" jint Java_javax_microedition_m3g_AnimationController_nCreate();
extern "C" void Java_javax_microedition_m3g_AnimationController_nSetActiveInterval();
extern "C" void Java_javax_microedition_m3g_AnimationController_nSetPosition();
extern "C" void Java_javax_microedition_m3g_AnimationController_nSetSpeed();
extern "C" void Java_javax_microedition_m3g_AnimationController_nSetWeight();
extern "C" jint Java_javax_microedition_m3g_AnimationTrack_nCreate();
extern "C" jint Java_javax_microedition_m3g_AnimationTrack_nGetSequence();
extern "C" jint Java_javax_microedition_m3g_AnimationTrack_nGetTargetProperty();
extern "C" void Java_javax_microedition_m3g_AnimationTrack_nSetController();
extern "C" jint Java_javax_microedition_m3g_Appearance_nCreate();
extern "C" jint Java_javax_microedition_m3g_Appearance_nGetCompositingMode();
extern "C" jint Java_javax_microedition_m3g_Appearance_nGetFog();
extern "C" jint Java_javax_microedition_m3g_Appearance_nGetLayer();
extern "C" jint Java_javax_microedition_m3g_Appearance_nGetMaterial();
extern "C" jint Java_javax_microedition_m3g_Appearance_nGetPolygonMode();
extern "C" jint Java_javax_microedition_m3g_Appearance_nGetTexture();
extern "C" void Java_javax_microedition_m3g_Appearance_nSetCompositingMode();
extern "C" void Java_javax_microedition_m3g_Appearance_nSetFog();
extern "C" void Java_javax_microedition_m3g_Appearance_nSetLayer();
extern "C" void Java_javax_microedition_m3g_Appearance_nSetMaterial();
extern "C" void Java_javax_microedition_m3g_Appearance_nSetPolygonMode();
extern "C" void Java_javax_microedition_m3g_Appearance_nSetTexture();
extern "C" jint Java_javax_microedition_m3g_Background_nCreate();
extern "C" void Java_javax_microedition_m3g_Background_nSetColor();
extern "C" void Java_javax_microedition_m3g_Background_nSetCrop();
extern "C" void Java_javax_microedition_m3g_Background_nSetEnable();
extern "C" void Java_javax_microedition_m3g_Background_nSetImage();
extern "C" void Java_javax_microedition_m3g_Background_nSetImageMode();
extern "C" jint Java_javax_microedition_m3g_Camera_nCreate();
extern "C" void Java_javax_microedition_m3g_Camera_nSetGeneric();
extern "C" void Java_javax_microedition_m3g_Camera_nSetParallel();
extern "C" void Java_javax_microedition_m3g_Camera_nSetPerspective();
extern "C" jint Java_javax_microedition_m3g_CompositingMode_nCreate();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nEnableAlphaWrite();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nEnableColorWrite();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nEnableDepthTest();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nEnableDepthWrite();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nSetAlphaThreshold();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nSetBlending();
extern "C" void Java_javax_microedition_m3g_CompositingMode_nSetDepthOffset();
extern "C" jint Java_javax_microedition_m3g_Fog_nCreate();
extern "C" void Java_javax_microedition_m3g_Fog_nSetColor();
extern "C" void Java_javax_microedition_m3g_Fog_nSetDensity();
extern "C" void Java_javax_microedition_m3g_Fog_nSetLinear();
extern "C" void Java_javax_microedition_m3g_Fog_nSetMode();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nAddLight();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nBind();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nClear();
extern "C" void Java_javax_microedition_m3g_Graphics3D_nClearLights();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nRelease();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nRenderImmediate();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nRenderNode();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nRenderWorld();
extern "C" jint Java_javax_microedition_m3g_Graphics3D_nSetCamera();
extern "C" void Java_javax_microedition_m3g_Graphics3D_nSetClipRect();
extern "C" void Java_javax_microedition_m3g_Graphics3D_nSetDepthRange();
extern "C" void Java_javax_microedition_m3g_Graphics3D_nSetViewport();
extern "C" void Java_javax_microedition_m3g_Group_nAddChild();
extern "C" jint Java_javax_microedition_m3g_Group_nCreate();
extern "C" jint Java_javax_microedition_m3g_Group_nGetChild();
extern "C" jint Java_javax_microedition_m3g_Group_nGetChildCount();
extern "C" void Java_javax_microedition_m3g_Group_nRemoveChild();
extern "C" void Java_javax_microedition_m3g_Image2D_nCommit();
extern "C" jint Java_javax_microedition_m3g_Image2D_nCreate();
extern "C" jint Java_javax_microedition_m3g_Image2D_nGetFormat();
extern "C" jint Java_javax_microedition_m3g_Image2D_nGetHeight();
extern "C" jint Java_javax_microedition_m3g_Image2D_nGetWidth();
extern "C" void Java_javax_microedition_m3g_Image2D_nSetImage();
extern "C" void Java_javax_microedition_m3g_Image2D_nSetPalette();
extern "C" void Java_javax_microedition_m3g_Image2D_nSetSubImage();
extern "C" jint Java_javax_microedition_m3g_KeyframeSequence_nCreate();
extern "C" jint Java_javax_microedition_m3g_KeyframeSequence_nGetDuration();
extern "C" jint Java_javax_microedition_m3g_KeyframeSequence_nGetKeyframeCount();
extern "C" void Java_javax_microedition_m3g_KeyframeSequence_nSetDuration();
extern "C" void Java_javax_microedition_m3g_KeyframeSequence_nSetKeyframe();
extern "C" void Java_javax_microedition_m3g_KeyframeSequence_nSetRepeatMode();
extern "C" void Java_javax_microedition_m3g_KeyframeSequence_nSetValidRange();
extern "C" jint Java_javax_microedition_m3g_Light_nCreate();
extern "C" void Java_javax_microedition_m3g_Light_nSetAttenuation();
extern "C" void Java_javax_microedition_m3g_Light_nSetColor();
extern "C" void Java_javax_microedition_m3g_Light_nSetIntensity();
extern "C" void Java_javax_microedition_m3g_Light_nSetMode();
extern "C" void Java_javax_microedition_m3g_Light_nSetSpotAngle();
extern "C" void Java_javax_microedition_m3g_Light_nSetSpotExponent();
extern "C" jint Java_javax_microedition_m3g_Loader_nLoadData();
extern "C" void Java_javax_microedition_m3g_Loader_nResultAbort();
extern "C" jint Java_javax_microedition_m3g_Loader_nResultClass();
extern "C" void Java_javax_microedition_m3g_Loader_nResultCommit();
extern "C" jint Java_javax_microedition_m3g_Loader_nResultHandle();
extern "C" jint Java_javax_microedition_m3g_Loader_nUserObjectCount();
extern "C" jint Java_javax_microedition_m3g_Loader_nUserObjectHandle();
extern "C" jint Java_javax_microedition_m3g_Loader_nUserParam();
extern "C" jint Java_javax_microedition_m3g_Loader_nUserParamCount();
extern "C" jint Java_javax_microedition_m3g_Loader_nUserParamLength();
extern "C" jint Java_javax_microedition_m3g_Material_nCreate();
extern "C" jint Java_javax_microedition_m3g_Material_nGetColor();
extern "C" void Java_javax_microedition_m3g_Material_nSetColor();
extern "C" void Java_javax_microedition_m3g_Material_nSetShininess();
extern "C" void Java_javax_microedition_m3g_Material_nSetVertexColorTracking();
extern "C" jint Java_javax_microedition_m3g_Mesh_nCreate();
extern "C" jint Java_javax_microedition_m3g_Mesh_nGetAppearance();
extern "C" jint Java_javax_microedition_m3g_Mesh_nGetIndexBuffer();
extern "C" jint Java_javax_microedition_m3g_Mesh_nGetSubmeshCount();
extern "C" jint Java_javax_microedition_m3g_Mesh_nGetVertexBuffer();
extern "C" void Java_javax_microedition_m3g_Mesh_nSetAppearance();
extern "C" jint Java_javax_microedition_m3g_MorphingMesh_nCreate();
extern "C" jint Java_javax_microedition_m3g_MorphingMesh_nGetMorphTarget();
extern "C" jint Java_javax_microedition_m3g_MorphingMesh_nGetMorphTargetCount();
extern "C" void Java_javax_microedition_m3g_MorphingMesh_nGetWeights();
extern "C" void Java_javax_microedition_m3g_MorphingMesh_nSetWeights();
extern "C" void Java_javax_microedition_m3g_Node_nAlign();
extern "C" void Java_javax_microedition_m3g_Node_nEnable();
extern "C" jint Java_javax_microedition_m3g_Node_nGetParent();
extern "C" jboolean Java_javax_microedition_m3g_Node_nGetTransformTo();
extern "C" void Java_javax_microedition_m3g_Node_nSetAlignment();
extern "C" void Java_javax_microedition_m3g_Node_nSetAlphaFactor();
extern "C" void Java_javax_microedition_m3g_Node_nSetScope();
extern "C" void Java_javax_microedition_m3g_Object3D_nAddAnimationTrack();
extern "C" void Java_javax_microedition_m3g_Object3D_nAddRef();
extern "C" jint Java_javax_microedition_m3g_Object3D_nAnimate();
extern "C" jint Java_javax_microedition_m3g_Object3D_nClassID();
extern "C" void Java_javax_microedition_m3g_Object3D_nDeleteRef();
extern "C" void Java_javax_microedition_m3g_Object3D_nDiag();
extern "C" jint Java_javax_microedition_m3g_Object3D_nDuplicate();
extern "C" jint Java_javax_microedition_m3g_Object3D_nFind();
extern "C" jint Java_javax_microedition_m3g_Object3D_nGetAnimationTrack();
extern "C" jint Java_javax_microedition_m3g_Object3D_nGetAnimationTrackCount();
extern "C" jint Java_javax_microedition_m3g_Object3D_nGetUserID();
extern "C" void Java_javax_microedition_m3g_Object3D_nRemoveAnimationTrack();
extern "C" void Java_javax_microedition_m3g_Object3D_nSetUserID();
extern "C" jint Java_javax_microedition_m3g_PolygonMode_nCreate();
extern "C" void Java_javax_microedition_m3g_PolygonMode_nSetCulling();
extern "C" void Java_javax_microedition_m3g_PolygonMode_nSetLocalCameraLighting();
extern "C" void Java_javax_microedition_m3g_PolygonMode_nSetPerspectiveCorrection();
extern "C" void Java_javax_microedition_m3g_PolygonMode_nSetShading();
extern "C" void Java_javax_microedition_m3g_PolygonMode_nSetTwoSidedLighting();
extern "C" void Java_javax_microedition_m3g_PolygonMode_nSetWinding();
extern "C" void Java_javax_microedition_m3g_SkinnedMesh_nAddTransform();
extern "C" jint Java_javax_microedition_m3g_SkinnedMesh_nCreate();
extern "C" jint Java_javax_microedition_m3g_SkinnedMesh_nGetSkeleton();
extern "C" jint Java_javax_microedition_m3g_Sprite3D_nCreate();
extern "C" void Java_javax_microedition_m3g_Sprite3D_nSetAppearance();
extern "C" void Java_javax_microedition_m3g_Sprite3D_nSetCrop();
extern "C" void Java_javax_microedition_m3g_Sprite3D_nSetImage();
extern "C" jint Java_javax_microedition_m3g_Texture2D_nCreate();
extern "C" jint Java_javax_microedition_m3g_Texture2D_nGetImage();
extern "C" void Java_javax_microedition_m3g_Texture2D_nSetBlendColor();
extern "C" void Java_javax_microedition_m3g_Texture2D_nSetBlending();
extern "C" void Java_javax_microedition_m3g_Texture2D_nSetFiltering();
extern "C" void Java_javax_microedition_m3g_Texture2D_nSetImage();
extern "C" void Java_javax_microedition_m3g_Texture2D_nSetWrapping();
extern "C" void Java_javax_microedition_m3g_Transformable_nGetCompositeTransform();
extern "C" void Java_javax_microedition_m3g_Transformable_nGetOrientation();
extern "C" void Java_javax_microedition_m3g_Transformable_nGetScale();
extern "C" void Java_javax_microedition_m3g_Transformable_nGetTransform();
extern "C" void Java_javax_microedition_m3g_Transformable_nGetTranslation();
extern "C" void Java_javax_microedition_m3g_Transformable_nPostRotate();
extern "C" void Java_javax_microedition_m3g_Transformable_nPreRotate();
extern "C" void Java_javax_microedition_m3g_Transformable_nScale();
extern "C" void Java_javax_microedition_m3g_Transformable_nSetOrientation();
extern "C" void Java_javax_microedition_m3g_Transformable_nSetScale();
extern "C" void Java_javax_microedition_m3g_Transformable_nSetTransform();
extern "C" void Java_javax_microedition_m3g_Transformable_nSetTranslation();
extern "C" void Java_javax_microedition_m3g_Transformable_nTranslate();
extern "C" jint Java_javax_microedition_m3g_TriangleStripArray_nCreateExplicit();
extern "C" jint Java_javax_microedition_m3g_TriangleStripArray_nCreateImplicit();
extern "C" jint Java_javax_microedition_m3g_VertexArray_nCreate();
extern "C" void Java_javax_microedition_m3g_VertexArray_nSetByte();
extern "C" void Java_javax_microedition_m3g_VertexArray_nSetShort();
extern "C" jint Java_javax_microedition_m3g_VertexBuffer_nCreate();
extern "C" jint Java_javax_microedition_m3g_VertexBuffer_nGetVertexCount();
extern "C" void Java_javax_microedition_m3g_VertexBuffer_nSetColors();
extern "C" void Java_javax_microedition_m3g_VertexBuffer_nSetDefaultColor();
extern "C" void Java_javax_microedition_m3g_VertexBuffer_nSetNormals();
extern "C" void Java_javax_microedition_m3g_VertexBuffer_nSetPositions();
extern "C" void Java_javax_microedition_m3g_VertexBuffer_nSetTexCoords();
extern "C" jint Java_javax_microedition_m3g_World_nCreate();
extern "C" jint Java_javax_microedition_m3g_World_nGetActiveCamera();
extern "C" jint Java_javax_microedition_m3g_World_nGetBackground();
extern "C" void Java_javax_microedition_m3g_World_nSetActiveCamera();
extern "C" void Java_javax_microedition_m3g_World_nSetBackground();


static const JvmNativeFunction com_nokia_mid_ui_DirectGraphicsImpl_natives[] = {
  JVM_NATIVE("drawPixels",    "([B[BIIIIIIII)V",       Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPixels___3B_3BIIIIIIII),
  JVM_NATIVE("drawPixels",    "([IZIIIIIIII)V",        Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPixels___3IZIIIIIIII),
  JVM_NATIVE("drawPixels",    "([SZIIIIIIII)V",        Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPixels___3SZIIIIIIII),
  JVM_NATIVE("drawPolygon",   "([II[IIII)V",           Java_com_nokia_mid_ui_DirectGraphicsImpl_drawPolygon),
  JVM_NATIVE("fillPolygon",   "([II[IIII)V",           Java_com_nokia_mid_ui_DirectGraphicsImpl_fillPolygon),
  JVM_NATIVE("getPixels",     "([B[BIIIIIII)V",        Java_com_nokia_mid_ui_DirectGraphicsImpl_getPixels___3B_3BIIIIIII),
  JVM_NATIVE("getPixels",     "([IIIIIIII)V",          Java_com_nokia_mid_ui_DirectGraphicsImpl_getPixels___3IIIIIIII),
  JVM_NATIVE("getPixels",     "([SIIIIIII)V",          Java_com_nokia_mid_ui_DirectGraphicsImpl_getPixels___3SIIIIIII),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_pspkvm_keypad_KeyMapInfo_natives[] = {
  JVM_NATIVE("getCurrentKeyMapForNativeControl","(IZ)I",                 Java_com_pspkvm_keypad_KeyMapInfo_getCurrentKeyMapForNativeControl),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_pspkvm_keypad_RawState_natives[] = {
  JVM_NATIVE("getAnalogX",    "()I",                   Java_com_pspkvm_keypad_RawState_getAnalogX),
  JVM_NATIVE("getAnalogY",    "()I",                   Java_com_pspkvm_keypad_RawState_getAnalogY),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_pspkvm_system_Power_natives[] = {
  JVM_NATIVE("getBatteryLifePercent","()I",                   Java_com_pspkvm_system_Power_getBatteryLifePercent),
  JVM_NATIVE("getBatteryLifeTime","()I",                   Java_com_pspkvm_system_Power_getBatteryLifeTime),
  JVM_NATIVE("getBatteryTemp","()I",                   Java_com_pspkvm_system_Power_getBatteryTemp),
  JVM_NATIVE("getBatteryVolt","()I",                   Java_com_pspkvm_system_Power_getBatteryVolt),
  JVM_NATIVE("getBusClockFrequency","()I",                   Java_com_pspkvm_system_Power_getBusClockFrequency),
  JVM_NATIVE("getCpuClockFrequency","()I",                   Java_com_pspkvm_system_Power_getCpuClockFrequency),
  JVM_NATIVE("isBatteryCharging","()Z",                   Java_com_pspkvm_system_Power_isBatteryCharging),
  JVM_NATIVE("isBatteryExist","()Z",                   Java_com_pspkvm_system_Power_isBatteryExist),
  JVM_NATIVE("isLowBattery",  "()Z",                   Java_com_pspkvm_system_Power_isLowBattery),
  JVM_NATIVE("isPowerOnline", "()Z",                   Java_com_pspkvm_system_Power_isPowerOnline),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_pspkvm_system_VMSettings_natives[] = {
  JVM_NATIVE("commit",        "()V",                   Java_com_pspkvm_system_VMSettings_commit),
  JVM_NATIVE("get",           "(Ljava/lang/String;)Ljava/lang/String;", Java_com_pspkvm_system_VMSettings_get),
  JVM_NATIVE("set",           "(Ljava/lang/String;Ljava/lang/String;)V", Java_com_pspkvm_system_VMSettings_set),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_pspkvm_system_WifiStatus_natives[] = {
  JVM_NATIVE("getBSSID",      "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getBSSID),
  JVM_NATIVE("getChannel",    "()I",                   Java_com_pspkvm_system_WifiStatus_getChannel),
  JVM_NATIVE("getEAPType",    "()I",                   Java_com_pspkvm_system_WifiStatus_getEAPType),
  JVM_NATIVE("getGateway",    "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getGateway),
  JVM_NATIVE("getIP",         "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getIP),
  JVM_NATIVE("getMACAddr",    "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getMACAddr),
  JVM_NATIVE("getPowerSave",  "()I",                   Java_com_pspkvm_system_WifiStatus_getPowerSave),
  JVM_NATIVE("getPrimaryDNS", "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getPrimaryDNS),
  JVM_NATIVE("getProfileName","()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getProfileName),
  JVM_NATIVE("getProxyPort",  "()I",                   Java_com_pspkvm_system_WifiStatus_getProxyPort),
  JVM_NATIVE("getProxyURL",   "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getProxyURL),
  JVM_NATIVE("getSSID",       "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getSSID),
  JVM_NATIVE("getSecondaryDNS","()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getSecondaryDNS),
  JVM_NATIVE("getSecurityType","()I",                   Java_com_pspkvm_system_WifiStatus_getSecurityType),
  JVM_NATIVE("getSignalStrength","()I",                   Java_com_pspkvm_system_WifiStatus_getSignalStrength),
  JVM_NATIVE("getStartBrowser","()I",                   Java_com_pspkvm_system_WifiStatus_getStartBrowser),
  JVM_NATIVE("getSubnetMask", "()Ljava/lang/String;",  Java_com_pspkvm_system_WifiStatus_getSubnetMask),
  JVM_NATIVE("getUseProxy",   "()I",                   Java_com_pspkvm_system_WifiStatus_getUseProxy),
  JVM_NATIVE("getUseWiFiSP",  "()I",                   Java_com_pspkvm_system_WifiStatus_getUseWiFiSP),
  JVM_NATIVE("isPowerOn",     "()Z",                   Java_com_pspkvm_system_WifiStatus_isPowerOn),
  JVM_NATIVE("isSwitchOn",    "()Z",                   Java_com_pspkvm_system_WifiStatus_isSwitchOn),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldc_i18n_j2me_Conv_natives[] = {
  JVM_NATIVE("byteToChar",    "(I[BII[CII)I",          Java_com_sun_cldc_i18n_j2me_Conv_byteToChar),
  JVM_NATIVE("charToByte",    "(I[CII[BII)I",          Java_com_sun_cldc_i18n_j2me_Conv_charToByte),
  JVM_NATIVE("getByteLength", "(I[BII)I",              Java_com_sun_cldc_i18n_j2me_Conv_getByteLength),
  JVM_NATIVE("getHandler",    "(Ljava/lang/String;)I", Java_com_sun_cldc_i18n_j2me_Conv_getHandler),
  JVM_NATIVE("getMaxByteLength","(I)I",                  Java_com_sun_cldc_i18n_j2me_Conv_getMaxByteLength),
  JVM_NATIVE("sizeOfByteInUnicode","(I[BII)I",              Java_com_sun_cldc_i18n_j2me_Conv_sizeOfByteInUnicode),
  JVM_NATIVE("sizeOfUnicodeInByte","(I[CII)I",              Java_com_sun_cldc_i18n_j2me_Conv_sizeOfUnicodeInByte),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldc_io_ResourceInputStream_natives[] = {
  JVM_NATIVE("bytesRemain",   "(Ljava/lang/Object;)I", Java_com_sun_cldc_io_ResourceInputStream_bytesRemain),
  JVM_NATIVE("clone",         "(Ljava/lang/Object;)Ljava/lang/Object;", Java_com_sun_cldc_io_ResourceInputStream_clone),
  JVM_NATIVE("open",          "(Ljava/lang/String;)Ljava/lang/Object;", Java_com_sun_cldc_io_ResourceInputStream_open),
  JVM_NATIVE("readByte",      "(Ljava/lang/Object;)I", Java_com_sun_cldc_io_ResourceInputStream_readByte),
  JVM_NATIVE("readBytes",     "(Ljava/lang/Object;[BII)I", Java_com_sun_cldc_io_ResourceInputStream_readBytes),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldc_isolate_Isolate_natives[] = {
  JVM_NATIVE("attachDebugger0","(Lcom/sun/cldc/isolate/Isolate;)V", Java_com_sun_cldc_isolate_Isolate_attachDebugger0),
  JVM_NATIVE("currentIsolate0","()Lcom/sun/cldc/isolate/Isolate;", Java_com_sun_cldc_isolate_Isolate_currentIsolate0),
  JVM_NATIVE("exitCode0",     "()I",                   Java_com_sun_cldc_isolate_Isolate_exitCode0),
  JVM_NATIVE("getIsolates0",  "()[Lcom/sun/cldc/isolate/Isolate;", Java_com_sun_cldc_isolate_Isolate_getIsolates0),
  JVM_NATIVE("getStatus",     "()I",                   Java_com_sun_cldc_isolate_Isolate_getStatus),
  JVM_NATIVE("id0",           "()I",                   Java_com_sun_cldc_isolate_Isolate_id0),
  JVM_NATIVE("isSuspended0",  "()I",                   Java_com_sun_cldc_isolate_Isolate_isSuspended0),
  JVM_NATIVE("nativeStart",   "()V",                   Java_com_sun_cldc_isolate_Isolate_nativeStart),
  JVM_NATIVE("notifyStatus",  "()V",                   Java_com_sun_cldc_isolate_Isolate_notifyStatus),
  JVM_NATIVE("registerNewIsolate","()V",                   Java_com_sun_cldc_isolate_Isolate_registerNewIsolate),
  JVM_NATIVE("resume0",       "()V",                   Java_com_sun_cldc_isolate_Isolate_resume0),
  JVM_NATIVE("setPriority0",  "(I)V",                  Java_com_sun_cldc_isolate_Isolate_setPriority0),
  JVM_NATIVE("setProfile",    "(Ljava/lang/String;)V", Java_com_sun_cldc_isolate_Isolate_setProfile),
  JVM_NATIVE("stop",          "(II)V",                 Java_com_sun_cldc_isolate_Isolate_stop),
  JVM_NATIVE("suspend0",      "()V",                   Java_com_sun_cldc_isolate_Isolate_suspend0),
  JVM_NATIVE("usedMemory0",   "()I",                   Java_com_sun_cldc_isolate_Isolate_usedMemory0),
  JVM_NATIVE("waitStatus",    "(I)V",                  Java_com_sun_cldc_isolate_Isolate_waitStatus),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldc_util_SemaphoreLock_natives[] = {
  JVM_NATIVE("acquire",       "()V",                   Java_com_sun_cldc_util_SemaphoreLock_acquire),
  JVM_NATIVE("release",       "()V",                   Java_com_sun_cldc_util_SemaphoreLock_release),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldchi_io_ConsoleOutputStream_natives[] = {
  JVM_NATIVE("write",         "(I)V",                  Java_com_sun_cldchi_io_ConsoleOutputStream_write),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldchi_jvm_FileDescriptor_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_cldchi_jvm_FileDescriptor_finalize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldchi_jvm_JVM_natives[] = {
  JVM_NATIVE("cancelImageCreation","()V",                   Java_com_sun_cldchi_jvm_JVM_cancelImageCreation),
  JVM_NATIVE("createAppImage0","()Z",                   Java_com_sun_cldchi_jvm_JVM_createAppImage0),
  JVM_NATIVE("createSysImage","()V",                   Java_com_sun_cldchi_jvm_JVM_createSysImage),
  JVM_NATIVE("getAppImageProgress","()I",                   Java_com_sun_cldchi_jvm_JVM_getAppImageProgress),
  JVM_NATIVE("loadLibrary",   "(Ljava/lang/String;)V", Java_com_sun_cldchi_jvm_JVM_loadLibrary),
  JVM_NATIVE("setLogChannel", "(I)V",                  Java_com_sun_cldchi_jvm_JVM_setLogChannel),
  JVM_NATIVE("startAppImage", "([C[CI)V",              Java_com_sun_cldchi_jvm_JVM_startAppImage),
  JVM_NATIVE("verifyNextChunk","(Ljava/lang/String;II)I", Java_com_sun_cldchi_jvm_JVM_verifyNextChunk),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_cldchi_jvm_JVM_entries[] = {
  JVM_ENTRY("unchecked_byte_arraycopy", "([BI[BII)V", native_jvm_unchecked_byte_arraycopy_entry),
  JVM_ENTRY("unchecked_char_arraycopy", "([CI[CII)V", native_jvm_unchecked_char_arraycopy_entry),
  JVM_ENTRY("unchecked_int_arraycopy", "([II[III)V", native_jvm_unchecked_int_arraycopy_entry),
  JVM_ENTRY("unchecked_long_arraycopy", "([JI[JII)V", native_jvm_unchecked_long_arraycopy_entry),
  JVM_ENTRY("unchecked_obj_arraycopy", "([Ljava/lang/Object;I[Ljava/lang/Object;II)V", native_jvm_unchecked_obj_arraycopy_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_j2me_location_LocationInfo_natives[] = {
  JVM_NATIVE("initNativeClass","()V",                   Java_com_sun_j2me_location_LocationInfo_initNativeClass),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_j2me_location_LocationProviderInfo_natives[] = {
  JVM_NATIVE("initNativeClass","()V",                   Java_com_sun_j2me_location_LocationProviderInfo_initNativeClass),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_j2me_location_PlatformLocationProvider_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_j2me_location_PlatformLocationProvider_finalize),
  JVM_NATIVE("getCriteria",   "(Ljava/lang/String;Lcom/sun/j2me/location/LocationProviderInfo;)Z", Java_com_sun_j2me_location_PlatformLocationProvider_getCriteria),
  JVM_NATIVE("getLastKnownLocationImpl","(Lcom/sun/j2me/location/LocationInfo;)Z", Java_com_sun_j2me_location_PlatformLocationProvider_getLastKnownLocationImpl),
  JVM_NATIVE("getLastLocationImpl","(ILcom/sun/j2me/location/LocationInfo;)Z", Java_com_sun_j2me_location_PlatformLocationProvider_getLastLocationImpl),
  JVM_NATIVE("getListOfLocationProviders","()Ljava/lang/String;",  Java_com_sun_j2me_location_PlatformLocationProvider_getListOfLocationProviders),
  JVM_NATIVE("getStateImpl",  "(I)I",                  Java_com_sun_j2me_location_PlatformLocationProvider_getStateImpl),
  JVM_NATIVE("open",          "(Ljava/lang/String;)I", Java_com_sun_j2me_location_PlatformLocationProvider_open),
  JVM_NATIVE("receiveNewLocationImpl","(IJ)Z",                 Java_com_sun_j2me_location_PlatformLocationProvider_receiveNewLocationImpl),
  JVM_NATIVE("resetImpl",     "(I)V",                  Java_com_sun_j2me_location_PlatformLocationProvider_resetImpl),
  JVM_NATIVE("waitForNewLocation","(IJ)Z",                 Java_com_sun_j2me_location_PlatformLocationProvider_waitForNewLocation),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_appmanager_WifiSelector_natives[] = {
  JVM_NATIVE("connect",       "(I)I",                  Java_com_sun_midp_appmanager_WifiSelector_connect),
  JVM_NATIVE("disconnect",    "()V",                   Java_com_sun_midp_appmanager_WifiSelector_disconnect),
  JVM_NATIVE("getConnectState","()I",                   Java_com_sun_midp_appmanager_WifiSelector_getConnectState),
  JVM_NATIVE("lookupWifiProfile","(I)Ljava/lang/String;", Java_com_sun_midp_appmanager_WifiSelector_lookupWifiProfile),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_chameleon_input_InputModeFactory_natives[] = {
  JVM_NATIVE("getInputModeIds","()[I",                  Java_com_sun_midp_chameleon_input_InputModeFactory_getInputModeIds),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_chameleon_input_NativeInputMode_natives[] = {
  JVM_NATIVE("beginInput0",   "(Lcom/sun/midp/chameleon/input/InputModeMediator;Ljava/lang/String;I)V", Java_com_sun_midp_chameleon_input_NativeInputMode_beginInput0),
  JVM_NATIVE("endInput0",     "()V",                   Java_com_sun_midp_chameleon_input_NativeInputMode_endInput0),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_chameleon_input_NativeInputMode_finalize),
  JVM_NATIVE("getCommandName","()Ljava/lang/String;",  Java_com_sun_midp_chameleon_input_NativeInputMode_getCommandName),
  JVM_NATIVE("getMatchList",  "()[Ljava/lang/String;", Java_com_sun_midp_chameleon_input_NativeInputMode_getMatchList),
  JVM_NATIVE("getName",       "()Ljava/lang/String;",  Java_com_sun_midp_chameleon_input_NativeInputMode_getName),
  JVM_NATIVE("getNextMatch",  "()Ljava/lang/String;",  Java_com_sun_midp_chameleon_input_NativeInputMode_getNextMatch),
  JVM_NATIVE("getPendingChar","()C",                   Java_com_sun_midp_chameleon_input_NativeInputMode_getPendingChar),
  JVM_NATIVE("hasMoreMatches","()Z",                   Java_com_sun_midp_chameleon_input_NativeInputMode_hasMoreMatches),
  JVM_NATIVE("initialize",    "(I)I",                  Java_com_sun_midp_chameleon_input_NativeInputMode_initialize),
  JVM_NATIVE("processKey0",   "(IZI[I)Ljava/lang/String;", Java_com_sun_midp_chameleon_input_NativeInputMode_processKey0),
  JVM_NATIVE("supportsConstraints","(I)Z",                  Java_com_sun_midp_chameleon_input_NativeInputMode_supportsConstraints),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_chameleon_skins_resources_LoadedSkinData_natives[] = {
  JVM_NATIVE("beginReadingSkinFile","(Ljava/lang/String;)V", Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_beginReadingSkinFile),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_finalize),
  JVM_NATIVE("finishReadingSkinFile","()I",                   Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_finishReadingSkinFile),
  JVM_NATIVE("readByteArray", "(I)[B",                 Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_readByteArray),
  JVM_NATIVE("readIntArray",  "()[I",                  Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_readIntArray),
  JVM_NATIVE("readStringArray","()[Ljava/lang/String;", Java_com_sun_midp_chameleon_skins_resources_LoadedSkinData_readStringArray),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_chameleon_skins_resources_LoadedSkinResources_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_chameleon_skins_resources_LoadedSkinResources_finalize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_chameleon_skins_resources_SkinResources_natives[] = {
  JVM_NATIVE("getRomizedImageDataArrayLength","(I)I",                  Java_com_sun_midp_chameleon_skins_resources_SkinResources_getRomizedImageDataArrayLength),
  JVM_NATIVE("getRomizedImageDataArrayPtr","(I)I",                  Java_com_sun_midp_chameleon_skins_resources_SkinResources_getRomizedImageDataArrayPtr),
  JVM_NATIVE("getSharedResourcePool","()Ljava/lang/Object;",  Java_com_sun_midp_chameleon_skins_resources_SkinResources_getSharedResourcePool),
  JVM_NATIVE("getSharedSkinData","()Ljava/lang/Object;",  Java_com_sun_midp_chameleon_skins_resources_SkinResources_getSharedSkinData),
  JVM_NATIVE("ifLoadAllResources","()Z",                   Java_com_sun_midp_chameleon_skins_resources_SkinResources_ifLoadAllResources),
  JVM_NATIVE("shareResourcePool","(Ljava/lang/Object;)V", Java_com_sun_midp_chameleon_skins_resources_SkinResources_shareResourcePool),
  JVM_NATIVE("shareSkinData", "(Ljava/lang/Object;)V", Java_com_sun_midp_chameleon_skins_resources_SkinResources_shareSkinData),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_crypto_ARC4_natives[] = {
  JVM_NATIVE("nativetx",      "([B[I[I[BII[BI)V",      Java_com_sun_midp_crypto_ARC4_nativetx),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_crypto_MD2_natives[] = {
  JVM_NATIVE("nativeFinal",   "([BII[BI[I[I[I[B)V",    Java_com_sun_midp_crypto_MD2_nativeFinal),
  JVM_NATIVE("nativeUpdate",  "([BII[I[I[I[B)V",       Java_com_sun_midp_crypto_MD2_nativeUpdate),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_crypto_MD5_natives[] = {
  JVM_NATIVE("nativeFinal",   "([BII[BI[I[I[I[I)V",    Java_com_sun_midp_crypto_MD5_nativeFinal),
  JVM_NATIVE("nativeUpdate",  "([BII[I[I[I[I)V",       Java_com_sun_midp_crypto_MD5_nativeUpdate),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_crypto_RSA_natives[] = {
  JVM_NATIVE("modExp",        "([B[B[B[B)I",           Java_com_sun_midp_crypto_RSA_modExp),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_crypto_SHA_natives[] = {
  JVM_NATIVE("nativeFinal",   "([BII[BI[I[I[I[I)V",    Java_com_sun_midp_crypto_SHA_nativeFinal),
  JVM_NATIVE("nativeUpdate",  "([BII[I[I[I[I)V",       Java_com_sun_midp_crypto_SHA_nativeUpdate),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_events_EventQueue_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_events_EventQueue_finalize),
  JVM_NATIVE("getNativeEventQueueHandle","()I",                   Java_com_sun_midp_events_EventQueue_getNativeEventQueueHandle),
  JVM_NATIVE("handleFatalError","(Ljava/lang/Throwable;)V", Java_com_sun_midp_events_EventQueue_handleFatalError),
  JVM_NATIVE("resetNativeEventQueue","()V",                   Java_com_sun_midp_events_EventQueue_resetNativeEventQueue),
  JVM_NATIVE("sendNativeEventToIsolate","(Lcom/sun/midp/events/NativeEvent;I)V", Java_com_sun_midp_events_EventQueue_sendNativeEventToIsolate),
  JVM_NATIVE("sendShutdownEvent","()V",                   Java_com_sun_midp_events_EventQueue_sendShutdownEvent),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_events_NativeEventMonitor_natives[] = {
  JVM_NATIVE("readNativeEvent","(Lcom/sun/midp/events/NativeEvent;)Z", Java_com_sun_midp_events_NativeEventMonitor_readNativeEvent),
  JVM_NATIVE("waitForNativeEvent","(Lcom/sun/midp/events/NativeEvent;)I", Java_com_sun_midp_events_NativeEventMonitor_waitForNativeEvent),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_installer_DeviceDesc_natives[] = {
  JVM_NATIVE("devIdToDispId", "(I)I",                  Java_com_sun_midp_installer_DeviceDesc_devIdToDispId),
  JVM_NATIVE("dispIdToDevId", "(I)I",                  Java_com_sun_midp_installer_DeviceDesc_dispIdToDevId),
  JVM_NATIVE("getCurrentDevice0","()I",                   Java_com_sun_midp_installer_DeviceDesc_getCurrentDevice0),
  JVM_NATIVE("getDefaultKeymap0","(I)I",                  Java_com_sun_midp_installer_DeviceDesc_getDefaultKeymap0),
  JVM_NATIVE("getDeviceHeight0","(I)I",                  Java_com_sun_midp_installer_DeviceDesc_getDeviceHeight0),
  JVM_NATIVE("getDeviceKeyCode0","(II)I",                 Java_com_sun_midp_installer_DeviceDesc_getDeviceKeyCode0),
  JVM_NATIVE("getDeviceName0","(I)Ljava/lang/String;", Java_com_sun_midp_installer_DeviceDesc_getDeviceName0),
  JVM_NATIVE("getDevicePropid0","(I)Ljava/lang/String;", Java_com_sun_midp_installer_DeviceDesc_getDevicePropid0),
  JVM_NATIVE("getDeviceWidth0","(I)I",                  Java_com_sun_midp_installer_DeviceDesc_getDeviceWidth0),
  JVM_NATIVE("getDevicesNumber0","()I",                   Java_com_sun_midp_installer_DeviceDesc_getDevicesNumber0),
  JVM_NATIVE("getJavaKeyNumber0","()I",                   Java_com_sun_midp_installer_DeviceDesc_getJavaKeyNumber0),
  JVM_NATIVE("resetKeymap0",  "()V",                   Java_com_sun_midp_installer_DeviceDesc_resetKeymap0),
  JVM_NATIVE("setCurrentCPUSpeed0","(I)V",                  Java_com_sun_midp_installer_DeviceDesc_setCurrentCPUSpeed0),
  JVM_NATIVE("setCurrentDevice0","(I)V",                  Java_com_sun_midp_installer_DeviceDesc_setCurrentDevice0),
  JVM_NATIVE("setDefaultKeymap0","()V",                   Java_com_sun_midp_installer_DeviceDesc_setDefaultKeymap0),
  JVM_NATIVE("setKeymap0",    "(II)V",                 Java_com_sun_midp_installer_DeviceDesc_setKeymap0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_installer_OtaNotifier_natives[] = {
  JVM_NATIVE("addInstallNotification","(ILjava/lang/String;)V", Java_com_sun_midp_installer_OtaNotifier_addInstallNotification),
  JVM_NATIVE("fillDeleteNotificationListForRetry","([Lcom/sun/midp/installer/PendingNotification;)V", Java_com_sun_midp_installer_OtaNotifier_fillDeleteNotificationListForRetry),
  JVM_NATIVE("getInstallNotificationForRetry","(ILcom/sun/midp/installer/PendingNotification;)Z", Java_com_sun_midp_installer_OtaNotifier_getInstallNotificationForRetry),
  JVM_NATIVE("getNumberOfDeleteNotifications","()I",                   Java_com_sun_midp_installer_OtaNotifier_getNumberOfDeleteNotifications),
  JVM_NATIVE("removeDeleteNotification","(I)V",                  Java_com_sun_midp_installer_OtaNotifier_removeDeleteNotification),
  JVM_NATIVE("removeInstallNotification","(I)V",                  Java_com_sun_midp_installer_OtaNotifier_removeInstallNotification),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_installer_SuiteDownloadInfo_natives[] = {
  JVM_NATIVE("closeDir",      "(I)V",                  Java_com_sun_midp_installer_SuiteDownloadInfo_closeDir),
  JVM_NATIVE("convert2lable0","(Ljava/lang/String;)Ljava/lang/String;", Java_com_sun_midp_installer_SuiteDownloadInfo_convert2lable0),
  JVM_NATIVE("nextFileInDir0","(Ljava/lang/String;I)Ljava/lang/String;", Java_com_sun_midp_installer_SuiteDownloadInfo_nextFileInDir0),
  JVM_NATIVE("openDir",       "(Ljava/lang/String;)I", Java_com_sun_midp_installer_SuiteDownloadInfo_openDir),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_NetworkConnectionBase_natives[] = {
  JVM_NATIVE("initializeInternal","()V",                   Java_com_sun_midp_io_NetworkConnectionBase_initializeInternal),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_comm_Protocol_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_io_j2me_comm_Protocol_finalize),
  JVM_NATIVE("native_close",  "(I)V",                  Java_com_sun_midp_io_j2me_comm_Protocol_native_1close),
  JVM_NATIVE("native_configurePort","(III)V",                Java_com_sun_midp_io_j2me_comm_Protocol_native_1configurePort),
  JVM_NATIVE("native_openByName","(Ljava/lang/String;II)I", Java_com_sun_midp_io_j2me_comm_Protocol_native_1openByName),
  JVM_NATIVE("native_readBytes","(I[BII)I",              Java_com_sun_midp_io_j2me_comm_Protocol_native_1readBytes),
  JVM_NATIVE("native_writeBytes","(I[BII)I",              Java_com_sun_midp_io_j2me_comm_Protocol_native_1writeBytes),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_datagram_Protocol_natives[] = {
  JVM_NATIVE("addrToString",  "(I)Ljava/lang/String;", Java_com_sun_midp_io_j2me_datagram_Protocol_addrToString),
  JVM_NATIVE("close0",        "()V",                   Java_com_sun_midp_io_j2me_datagram_Protocol_close0),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_io_j2me_datagram_Protocol_finalize),
  JVM_NATIVE("getHost0",      "()Ljava/lang/String;",  Java_com_sun_midp_io_j2me_datagram_Protocol_getHost0),
  JVM_NATIVE("getIpNumber",   "(Ljava/lang/String;)I", Java_com_sun_midp_io_j2me_datagram_Protocol_getIpNumber),
  JVM_NATIVE("getMaximumLength0","()I",                   Java_com_sun_midp_io_j2me_datagram_Protocol_getMaximumLength0),
  JVM_NATIVE("getNominalLength0","()I",                   Java_com_sun_midp_io_j2me_datagram_Protocol_getNominalLength0),
  JVM_NATIVE("getPort0",      "()I",                   Java_com_sun_midp_io_j2me_datagram_Protocol_getPort0),
  JVM_NATIVE("open0",         "(II)V",                 Java_com_sun_midp_io_j2me_datagram_Protocol_open0),
  JVM_NATIVE("receive0",      "([BII)J",               Java_com_sun_midp_io_j2me_datagram_Protocol_receive0),
  JVM_NATIVE("send0",         "(II[BII)I",             Java_com_sun_midp_io_j2me_datagram_Protocol_send0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_file_DefaultFileHandler_natives[] = {
  JVM_NATIVE("availableSize", "()J",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_availableSize),
  JVM_NATIVE("canRead",       "()Z",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_canRead),
  JVM_NATIVE("canWrite",      "()Z",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_canWrite),
  JVM_NATIVE("close",         "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_close),
  JVM_NATIVE("closeDir",      "(J)V",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeDir),
  JVM_NATIVE("closeForRead",  "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeForRead),
  JVM_NATIVE("closeForReadWrite","()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeForReadWrite),
  JVM_NATIVE("closeForWrite", "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_closeForWrite),
  JVM_NATIVE("create",        "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_create),
  JVM_NATIVE("delete",        "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_delete),
  JVM_NATIVE("dirGetNextFile","(JZ)Ljava/lang/String;", Java_com_sun_midp_io_j2me_file_DefaultFileHandler_dirGetNextFile),
  JVM_NATIVE("directorySize", "(Z)J",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_directorySize),
  JVM_NATIVE("exists",        "()Z",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_exists),
  JVM_NATIVE("fileSize",      "()J",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_fileSize),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_finalize),
  JVM_NATIVE("flush",         "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_flush),
  JVM_NATIVE("getFileSeparator","()C",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getFileSeparator),
  JVM_NATIVE("getMountedRoots","()Ljava/lang/String;",  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getMountedRoots),
  JVM_NATIVE("getNativeName", "(Ljava/lang/String;J)J", Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getNativeName),
  JVM_NATIVE("getNativePathForRoot","(Ljava/lang/String;)Ljava/lang/String;", Java_com_sun_midp_io_j2me_file_DefaultFileHandler_getNativePathForRoot),
  JVM_NATIVE("illegalFileNameChars0","()Ljava/lang/String;",  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_illegalFileNameChars0),
  JVM_NATIVE("initialize",    "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_initialize),
  JVM_NATIVE("isDirectory",   "()Z",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_isDirectory),
  JVM_NATIVE("isHidden0",     "()Z",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_isHidden0),
  JVM_NATIVE("lastModified",  "()J",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_lastModified),
  JVM_NATIVE("mkdir",         "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_mkdir),
  JVM_NATIVE("openDir",       "()J",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_openDir),
  JVM_NATIVE("openForRead",   "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_openForRead),
  JVM_NATIVE("openForWrite",  "()V",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_openForWrite),
  JVM_NATIVE("positionForWrite","(J)V",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_positionForWrite),
  JVM_NATIVE("read",          "([BII)I",               Java_com_sun_midp_io_j2me_file_DefaultFileHandler_read),
  JVM_NATIVE("rename0",       "(Ljava/lang/String;)V", Java_com_sun_midp_io_j2me_file_DefaultFileHandler_rename0),
  JVM_NATIVE("setHidden0",    "(Z)V",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_setHidden0),
  JVM_NATIVE("setReadable",   "(Z)V",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_setReadable),
  JVM_NATIVE("setWritable",   "(Z)V",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_setWritable),
  JVM_NATIVE("totalSize",     "()J",                   Java_com_sun_midp_io_j2me_file_DefaultFileHandler_totalSize),
  JVM_NATIVE("truncate",      "(J)V",                  Java_com_sun_midp_io_j2me_file_DefaultFileHandler_truncate),
  JVM_NATIVE("write",         "([BII)I",               Java_com_sun_midp_io_j2me_file_DefaultFileHandler_write),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_push_ConnectionRegistry_natives[] = {
  JVM_NATIVE("add0",          "(Ljava/lang/String;)I", Java_com_sun_midp_io_j2me_push_ConnectionRegistry_add0),
  JVM_NATIVE("addAlarm0",     "([BJ)J",                Java_com_sun_midp_io_j2me_push_ConnectionRegistry_addAlarm0),
  JVM_NATIVE("checkInByHandle0","(I)V",                  Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByHandle0),
  JVM_NATIVE("checkInByMidlet0","(ILjava/lang/String;)V", Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByMidlet0),
  JVM_NATIVE("checkInByName0","([B)I",                 Java_com_sun_midp_io_j2me_push_ConnectionRegistry_checkInByName0),
  JVM_NATIVE("del0",          "(Ljava/lang/String;Ljava/lang/String;)I", Java_com_sun_midp_io_j2me_push_ConnectionRegistry_del0),
  JVM_NATIVE("delAllForSuite0","(I)V",                  Java_com_sun_midp_io_j2me_push_ConnectionRegistry_delAllForSuite0),
  JVM_NATIVE("getEntry0",     "([B[BI)I",              Java_com_sun_midp_io_j2me_push_ConnectionRegistry_getEntry0),
  JVM_NATIVE("getMIDlet0",    "(I[BI)I",               Java_com_sun_midp_io_j2me_push_ConnectionRegistry_getMIDlet0),
  JVM_NATIVE("list0",         "([BZ[BI)I",             Java_com_sun_midp_io_j2me_push_ConnectionRegistry_list0),
  JVM_NATIVE("poll0",         "(J)I",                  Java_com_sun_midp_io_j2me_push_ConnectionRegistry_poll0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_socket_Protocol_natives[] = {
  JVM_NATIVE("available0",    "()I",                   Java_com_sun_midp_io_j2me_socket_Protocol_available0),
  JVM_NATIVE("close0",        "()V",                   Java_com_sun_midp_io_j2me_socket_Protocol_close0),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_io_j2me_socket_Protocol_finalize),
  JVM_NATIVE("getHost0",      "(Z)Ljava/lang/String;", Java_com_sun_midp_io_j2me_socket_Protocol_getHost0),
  JVM_NATIVE("getIpNumber0",  "(Ljava/lang/String;[B)I", Java_com_sun_midp_io_j2me_socket_Protocol_getIpNumber0),
  JVM_NATIVE("getPort0",      "(Z)I",                  Java_com_sun_midp_io_j2me_socket_Protocol_getPort0),
  JVM_NATIVE("getSockOpt0",   "(I)I",                  Java_com_sun_midp_io_j2me_socket_Protocol_getSockOpt0),
  JVM_NATIVE("notifyClosedInput0","()V",                   Java_com_sun_midp_io_j2me_socket_Protocol_notifyClosedInput0),
  JVM_NATIVE("notifyClosedOutput0","()V",                   Java_com_sun_midp_io_j2me_socket_Protocol_notifyClosedOutput0),
  JVM_NATIVE("open0",         "([BI)V",                Java_com_sun_midp_io_j2me_socket_Protocol_open0),
  JVM_NATIVE("read0",         "([BII)I",               Java_com_sun_midp_io_j2me_socket_Protocol_read0),
  JVM_NATIVE("setSockOpt0",   "(II)V",                 Java_com_sun_midp_io_j2me_socket_Protocol_setSockOpt0),
  JVM_NATIVE("shutdownOutput0","()V",                   Java_com_sun_midp_io_j2me_socket_Protocol_shutdownOutput0),
  JVM_NATIVE("write0",        "([BII)I",               Java_com_sun_midp_io_j2me_socket_Protocol_write0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_storage_File_natives[] = {
  JVM_NATIVE("availableStorage","(I)J",                  Java_com_sun_midp_io_j2me_storage_File_availableStorage),
  JVM_NATIVE("deleteStorage", "(Ljava/lang/String;)V", Java_com_sun_midp_io_j2me_storage_File_deleteStorage),
  JVM_NATIVE("initConfigRoot","(I)Ljava/lang/String;", Java_com_sun_midp_io_j2me_storage_File_initConfigRoot),
  JVM_NATIVE("initStorageRoot","(I)Ljava/lang/String;", Java_com_sun_midp_io_j2me_storage_File_initStorageRoot),
  JVM_NATIVE("renameStorage", "(Ljava/lang/String;Ljava/lang/String;)V", Java_com_sun_midp_io_j2me_storage_File_renameStorage),
  JVM_NATIVE("storageExists", "(Ljava/lang/String;)Z", Java_com_sun_midp_io_j2me_storage_File_storageExists),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_io_j2me_storage_RandomAccessStream_natives[] = {
  JVM_NATIVE("close",         "(I)V",                  Java_com_sun_midp_io_j2me_storage_RandomAccessStream_close),
  JVM_NATIVE("commitWrite",   "(I)V",                  Java_com_sun_midp_io_j2me_storage_RandomAccessStream_commitWrite),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_io_j2me_storage_RandomAccessStream_finalize),
  JVM_NATIVE("open",          "(Ljava/lang/String;I)I", Java_com_sun_midp_io_j2me_storage_RandomAccessStream_open),
  JVM_NATIVE("position",      "(II)V",                 Java_com_sun_midp_io_j2me_storage_RandomAccessStream_position),
  JVM_NATIVE("read",          "(I[BII)I",              Java_com_sun_midp_io_j2me_storage_RandomAccessStream_read),
  JVM_NATIVE("sizeOf",        "(I)I",                  Java_com_sun_midp_io_j2me_storage_RandomAccessStream_sizeOf),
  JVM_NATIVE("truncateStream","(II)V",                 Java_com_sun_midp_io_j2me_storage_RandomAccessStream_truncateStream),
  JVM_NATIVE("write",         "(I[BII)V",              Java_com_sun_midp_io_j2me_storage_RandomAccessStream_write),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_jarutil_JarReader_natives[] = {
  JVM_NATIVE("readJarEntry0", "(Ljava/lang/String;Ljava/lang/String;)[B", Java_com_sun_midp_jarutil_JarReader_readJarEntry0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_jsr075_Initializer_natives[] = {
  JVM_NATIVE("cleanup",       "()V",                   Java_com_sun_midp_jsr075_Initializer_cleanup),
  JVM_NATIVE("init",          "()V",                   Java_com_sun_midp_jsr075_Initializer_init),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_l10n_LocalizedStringsBase_natives[] = {
  JVM_NATIVE("getContent",    "(I)Ljava/lang/String;", Java_com_sun_midp_l10n_LocalizedStringsBase_getContent),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_l10n_LocalizedStringsBasezhCN_natives[] = {
  JVM_NATIVE("getContent",    "(I)Ljava/lang/String;", Java_com_sun_midp_l10n_LocalizedStringsBasezhCN_getContent),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_lcdui_DisplayDeviceAccess_natives[] = {
  JVM_NATIVE("isBacklightSupported0","(I)Z",                  Java_com_sun_midp_lcdui_DisplayDeviceAccess_isBacklightSupported0),
  JVM_NATIVE("setDeviceScreenSize","(II)V",                 Java_com_sun_midp_lcdui_DisplayDeviceAccess_setDeviceScreenSize),
  JVM_NATIVE("toggleBacklight0","(I)Z",                  Java_com_sun_midp_lcdui_DisplayDeviceAccess_toggleBacklight0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_links_Link_natives[] = {
  JVM_NATIVE("close",         "()V",                   Java_com_sun_midp_links_Link_close),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_links_Link_finalize),
  JVM_NATIVE("init0",         "(II)V",                 Java_com_sun_midp_links_Link_init0),
  JVM_NATIVE("isOpen",        "()Z",                   Java_com_sun_midp_links_Link_isOpen),
  JVM_NATIVE("receive0",      "(Lcom/sun/midp/links/LinkMessage;Lcom/sun/midp/links/Link;)V", Java_com_sun_midp_links_Link_receive0),
  JVM_NATIVE("send0",         "(Lcom/sun/midp/links/LinkMessage;)V", Java_com_sun_midp_links_Link_send0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_links_LinkPortal_natives[] = {
  JVM_NATIVE("getLinkCount0", "()I",                   Java_com_sun_midp_links_LinkPortal_getLinkCount0),
  JVM_NATIVE("getLinks0",     "([Lcom/sun/midp/links/Link;)V", Java_com_sun_midp_links_LinkPortal_getLinks0),
  JVM_NATIVE("setLinks0",     "(I[Lcom/sun/midp/links/Link;)V", Java_com_sun_midp_links_LinkPortal_setLinks0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_log_LoggingBase_natives[] = {
  JVM_NATIVE("report",        "(IILjava/lang/String;)V", Java_com_sun_midp_log_LoggingBase_report),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_AppIsolateMIDletSuiteLoader_natives[] = {
  JVM_NATIVE("allocateReservedResources0","()Z",                   Java_com_sun_midp_main_AppIsolateMIDletSuiteLoader_allocateReservedResources0),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_main_AppIsolateMIDletSuiteLoader_finalize),
  JVM_NATIVE("handleFatalError","(Ljava/lang/Throwable;)V", Java_com_sun_midp_main_AppIsolateMIDletSuiteLoader_handleFatalError),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_CldcPlatformRequest_natives[] = {
  JVM_NATIVE("dispatchPlatformRequest","(Ljava/lang/String;)Z", Java_com_sun_midp_main_CldcPlatformRequest_dispatchPlatformRequest),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_CommandState_natives[] = {
  JVM_NATIVE("exitInternal",  "(I)V",                  Java_com_sun_midp_main_CommandState_exitInternal),
  JVM_NATIVE("restoreCommandState","(Lcom/sun/midp/main/CommandState;)V", Java_com_sun_midp_main_CommandState_restoreCommandState),
  JVM_NATIVE("saveCommandState","(Lcom/sun/midp/main/CommandState;)V", Java_com_sun_midp_main_CommandState_saveCommandState),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_Configuration_natives[] = {
  JVM_NATIVE("getProperty0",  "(Ljava/lang/String;)Ljava/lang/String;", Java_com_sun_midp_main_Configuration_getProperty0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_IndicatorManager_natives[] = {
  JVM_NATIVE("toggleHomeIcon0","(Z)V",                  Java_com_sun_midp_main_IndicatorManager_toggleHomeIcon0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_MIDletAppImageGenerator_natives[] = {
  JVM_NATIVE("removeAppImage","(Ljava/lang/String;)Z", Java_com_sun_midp_main_MIDletAppImageGenerator_removeAppImage),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_MIDletProxyList_natives[] = {
  JVM_NATIVE("notifyResumeAll0","()V",                   Java_com_sun_midp_main_MIDletProxyList_notifyResumeAll0),
  JVM_NATIVE("notifySuspendAll0","()V",                   Java_com_sun_midp_main_MIDletProxyList_notifySuspendAll0),
  JVM_NATIVE("setForegroundInNativeState","(II)V",                 Java_com_sun_midp_main_MIDletProxyList_setForegroundInNativeState),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_MIDletSuiteUtils_natives[] = {
  JVM_NATIVE("getAmsIsolateId","()I",                   Java_com_sun_midp_main_MIDletSuiteUtils_getAmsIsolateId),
  JVM_NATIVE("getIsolateId",  "()I",                   Java_com_sun_midp_main_MIDletSuiteUtils_getIsolateId),
  JVM_NATIVE("isAmsIsolate",  "()Z",                   Java_com_sun_midp_main_MIDletSuiteUtils_isAmsIsolate),
  JVM_NATIVE("registerAmsIsolateId","()V",                   Java_com_sun_midp_main_MIDletSuiteUtils_registerAmsIsolateId),
  JVM_NATIVE("vmBeginStartUp","(I)V",                  Java_com_sun_midp_main_MIDletSuiteUtils_vmBeginStartUp),
  JVM_NATIVE("vmEndStartUp",  "(I)V",                  Java_com_sun_midp_main_MIDletSuiteUtils_vmEndStartUp),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_main_MIDletSuiteVerifier_natives[] = {
  JVM_NATIVE("checkJarHash",  "(Ljava/lang/String;[B)Z", Java_com_sun_midp_main_MIDletSuiteVerifier_checkJarHash),
  JVM_NATIVE("getJarHash",    "(Ljava/lang/String;)[B", Java_com_sun_midp_main_MIDletSuiteVerifier_getJarHash),
  JVM_NATIVE("useClassVerifier","(Z)V",                  Java_com_sun_midp_main_MIDletSuiteVerifier_useClassVerifier),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_midletsuite_InstallInfo_natives[] = {
  JVM_NATIVE("load",          "()V",                   Java_com_sun_midp_midletsuite_InstallInfo_load),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_midletsuite_MIDletSuiteImpl_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_midletsuite_MIDletSuiteImpl_finalize),
  JVM_NATIVE("lockMIDletSuite","(IZ)V",                 Java_com_sun_midp_midletsuite_MIDletSuiteImpl_lockMIDletSuite),
  JVM_NATIVE("unlockMIDletSuite","(I)V",                  Java_com_sun_midp_midletsuite_MIDletSuiteImpl_unlockMIDletSuite),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_midletsuite_MIDletSuiteStorage_natives[] = {
  JVM_NATIVE("createSuiteID", "()I",                   Java_com_sun_midp_midletsuite_MIDletSuiteStorage_createSuiteID),
  JVM_NATIVE("disable",       "(I)V",                  Java_com_sun_midp_midletsuite_MIDletSuiteStorage_disable),
  JVM_NATIVE("enable",        "(I)V",                  Java_com_sun_midp_midletsuite_MIDletSuiteStorage_enable),
  JVM_NATIVE("getMIDletSuiteIcon0","(ILjava/lang/String;)[B", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMIDletSuiteIcon0),
  JVM_NATIVE("getMIDletSuiteInfoImpl0","(ILcom/sun/midp/midletsuite/MIDletSuiteInfo;)V", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMIDletSuiteInfoImpl0),
  JVM_NATIVE("getMidletSuiteAppImagePath","(I)Ljava/lang/String;", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMidletSuiteAppImagePath),
  JVM_NATIVE("getMidletSuiteJarPath","(I)Ljava/lang/String;", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getMidletSuiteJarPath),
  JVM_NATIVE("getNumberOfSuites","()I",                   Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getNumberOfSuites),
  JVM_NATIVE("getStorageUsed","(I)I",                  Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getStorageUsed),
  JVM_NATIVE("getSuiteID",    "(Ljava/lang/String;Ljava/lang/String;)I", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getSuiteID),
  JVM_NATIVE("getSuiteList",  "([I)V",                 Java_com_sun_midp_midletsuite_MIDletSuiteStorage_getSuiteList),
  JVM_NATIVE("loadCachedIcon0","(ILjava/lang/String;)[B", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_loadCachedIcon0),
  JVM_NATIVE("loadSuitesIcons0","()I",                   Java_com_sun_midp_midletsuite_MIDletSuiteStorage_loadSuitesIcons0),
  JVM_NATIVE("nativeStoreSuite","(Lcom/sun/midp/midletsuite/InstallInfo;Lcom/sun/midp/midletsuite/SuiteSettings;Lcom/sun/midp/midletsuite/MIDletSuiteInfo;[Ljava/lang/String;[Ljava/lang/String;Z)V", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_nativeStoreSuite),
  JVM_NATIVE("remove0",       "(I)V",                  Java_com_sun_midp_midletsuite_MIDletSuiteStorage_remove0),
  JVM_NATIVE("storeSuiteVerifyHash","(I[B)V",                Java_com_sun_midp_midletsuite_MIDletSuiteStorage_storeSuiteVerifyHash),
  JVM_NATIVE("suiteExists",   "(I)Z",                  Java_com_sun_midp_midletsuite_MIDletSuiteStorage_suiteExists),
  JVM_NATIVE("suiteIdToString","(I)Ljava/lang/String;", Java_com_sun_midp_midletsuite_MIDletSuiteStorage_suiteIdToString),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_midletsuite_SuiteProperties_natives[] = {
  JVM_NATIVE("load",          "()[Ljava/lang/String;", Java_com_sun_midp_midletsuite_SuiteProperties_load),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_midletsuite_SuiteSettings_natives[] = {
  JVM_NATIVE("load0",         "()V",                   Java_com_sun_midp_midletsuite_SuiteSettings_load0),
  JVM_NATIVE("save0",         "(IBI[B)V",              Java_com_sun_midp_midletsuite_SuiteSettings_save0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_rms_RecordStoreFactory_natives[] = {
  JVM_NATIVE("suiteHasRmsData","(I)Z",                  Java_com_sun_midp_rms_RecordStoreFactory_suiteHasRmsData),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_rms_RecordStoreFile_natives[] = {
  JVM_NATIVE("closeFile",     "(I)V",                  Java_com_sun_midp_rms_RecordStoreFile_closeFile),
  JVM_NATIVE("commitWrite",   "(I)V",                  Java_com_sun_midp_rms_RecordStoreFile_commitWrite),
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_midp_rms_RecordStoreFile_finalize),
  JVM_NATIVE("getNumberOfStores","(I)I",                  Java_com_sun_midp_rms_RecordStoreFile_getNumberOfStores),
  JVM_NATIVE("getRecordStoreList","(I[Ljava/lang/String;)V", Java_com_sun_midp_rms_RecordStoreFile_getRecordStoreList),
  JVM_NATIVE("openRecordStoreFile","(ILjava/lang/String;I)I", Java_com_sun_midp_rms_RecordStoreFile_openRecordStoreFile),
  JVM_NATIVE("readBytes",     "(I[BII)I",              Java_com_sun_midp_rms_RecordStoreFile_readBytes),
  JVM_NATIVE("removeRecordStores","(I)V",                  Java_com_sun_midp_rms_RecordStoreFile_removeRecordStores),
  JVM_NATIVE("setPosition",   "(II)V",                 Java_com_sun_midp_rms_RecordStoreFile_setPosition),
  JVM_NATIVE("spaceAvailableNewRecordStore","(I)I",                  Java_com_sun_midp_rms_RecordStoreFile_spaceAvailableNewRecordStore),
  JVM_NATIVE("spaceAvailableRecordStore","(II)I",                 Java_com_sun_midp_rms_RecordStoreFile_spaceAvailableRecordStore),
  JVM_NATIVE("truncateFile",  "(II)V",                 Java_com_sun_midp_rms_RecordStoreFile_truncateFile),
  JVM_NATIVE("writeBytes",    "(I[BII)V",              Java_com_sun_midp_rms_RecordStoreFile_writeBytes),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_rms_RecordStoreUtil_natives[] = {
  JVM_NATIVE("deleteFile",    "(ILjava/lang/String;I)V", Java_com_sun_midp_rms_RecordStoreUtil_deleteFile),
  JVM_NATIVE("exists",        "(ILjava/lang/String;I)Z", Java_com_sun_midp_rms_RecordStoreUtil_exists),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_suspend_SuspendSystem_natives[] = {
  JVM_NATIVE("isResumePending","()Z",                   Java_com_sun_midp_suspend_SuspendSystem_isResumePending),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_suspend_SuspendSystem_MIDPSystem_natives[] = {
  JVM_NATIVE("allMidletsKilled","()Z",                   Java_com_sun_midp_suspend_SuspendSystem_00024MIDPSystem_allMidletsKilled),
  JVM_NATIVE("suspended0",    "(Z)V",                  Java_com_sun_midp_suspend_SuspendSystem_00024MIDPSystem_suspended0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_midp_util_ResourceHandler_natives[] = {
  JVM_NATIVE("loadRomizedResource0","(Ljava/lang/String;)[B", Java_com_sun_midp_util_ResourceHandler_loadRomizedResource0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_DefaultConfiguration_natives[] = {
  JVM_NATIVE("nIsAmrSupported","()Z",                   Java_com_sun_mmedia_DefaultConfiguration_nIsAmrSupported),
  JVM_NATIVE("nIsJtsSupported","()Z",                   Java_com_sun_mmedia_DefaultConfiguration_nIsJtsSupported),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_DirectMIDIControl_natives[] = {
  JVM_NATIVE("nGetBankList",  "(IZ[I)I",               Java_com_sun_mmedia_DirectMIDIControl_nGetBankList),
  JVM_NATIVE("nGetChannelVolume","(II)I",                 Java_com_sun_mmedia_DirectMIDIControl_nGetChannelVolume),
  JVM_NATIVE("nGetKeyName",   "(IIII[B)I",             Java_com_sun_mmedia_DirectMIDIControl_nGetKeyName),
  JVM_NATIVE("nGetMaxPitch",  "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetMaxPitch),
  JVM_NATIVE("nGetMaxRate",   "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetMaxRate),
  JVM_NATIVE("nGetMinPitch",  "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetMinPitch),
  JVM_NATIVE("nGetMinRate",   "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetMinRate),
  JVM_NATIVE("nGetPitch",     "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetPitch),
  JVM_NATIVE("nGetProgram",   "(II[I)I",               Java_com_sun_mmedia_DirectMIDIControl_nGetProgram),
  JVM_NATIVE("nGetProgramList","(II[I)I",               Java_com_sun_mmedia_DirectMIDIControl_nGetProgramList),
  JVM_NATIVE("nGetProgramName","(III[B)I",              Java_com_sun_mmedia_DirectMIDIControl_nGetProgramName),
  JVM_NATIVE("nGetRate",      "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetRate),
  JVM_NATIVE("nGetTempo",     "(I)I",                  Java_com_sun_mmedia_DirectMIDIControl_nGetTempo),
  JVM_NATIVE("nIsBankQuerySupported","(I)Z",                  Java_com_sun_mmedia_DirectMIDIControl_nIsBankQuerySupported),
  JVM_NATIVE("nLongMidiEvent","(I[BII)I",              Java_com_sun_mmedia_DirectMIDIControl_nLongMidiEvent),
  JVM_NATIVE("nSetChannelVolume","(III)V",                Java_com_sun_mmedia_DirectMIDIControl_nSetChannelVolume),
  JVM_NATIVE("nSetPitch",     "(II)I",                 Java_com_sun_mmedia_DirectMIDIControl_nSetPitch),
  JVM_NATIVE("nSetProgram",   "(IIII)V",               Java_com_sun_mmedia_DirectMIDIControl_nSetProgram),
  JVM_NATIVE("nSetRate",      "(II)I",                 Java_com_sun_mmedia_DirectMIDIControl_nSetRate),
  JVM_NATIVE("nSetTempo",     "(II)I",                 Java_com_sun_mmedia_DirectMIDIControl_nSetTempo),
  JVM_NATIVE("nShortMidiEvent","(IIII)V",               Java_com_sun_mmedia_DirectMIDIControl_nShortMidiEvent),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_DirectPlayer_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_mmedia_DirectPlayer_finalize),
  JVM_NATIVE("nAcquireDevice","(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nAcquireDevice),
  JVM_NATIVE("nBuffering",    "(ILjava/lang/Object;J)I", Java_com_sun_mmedia_DirectPlayer_nBuffering),
  JVM_NATIVE("nFlushBuffer",  "(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nFlushBuffer),
  JVM_NATIVE("nGetDuration",  "(I)I",                  Java_com_sun_mmedia_DirectPlayer_nGetDuration),
  JVM_NATIVE("nGetMediaTime", "(I)I",                  Java_com_sun_mmedia_DirectPlayer_nGetMediaTime),
  JVM_NATIVE("nInit",         "(IILjava/lang/String;Ljava/lang/String;J)I", Java_com_sun_mmedia_DirectPlayer_nInit),
  JVM_NATIVE("nIsNeedBuffering","(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nIsNeedBuffering),
  JVM_NATIVE("nIsSupportRecording","(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nIsSupportRecording),
  JVM_NATIVE("nPause",        "(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nPause),
  JVM_NATIVE("nReleaseDevice","(I)V",                  Java_com_sun_mmedia_DirectPlayer_nReleaseDevice),
  JVM_NATIVE("nResume",       "(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nResume),
  JVM_NATIVE("nSetMediaTime", "(IJ)I",                 Java_com_sun_mmedia_DirectPlayer_nSetMediaTime),
  JVM_NATIVE("nStart",        "(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nStart),
  JVM_NATIVE("nStop",         "(I)Z",                  Java_com_sun_mmedia_DirectPlayer_nStop),
  JVM_NATIVE("nSwitchToBackground","(II)Z",                 Java_com_sun_mmedia_DirectPlayer_nSwitchToBackground),
  JVM_NATIVE("nSwitchToForeground","(II)Z",                 Java_com_sun_mmedia_DirectPlayer_nSwitchToForeground),
  JVM_NATIVE("nTerm",         "(I)I",                  Java_com_sun_mmedia_DirectPlayer_nTerm),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_DirectRecord_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_mmedia_DirectRecord_finalize),
  JVM_NATIVE("nClose",        "(I)I",                  Java_com_sun_mmedia_DirectRecord_nClose),
  JVM_NATIVE("nCommit",       "(I)I",                  Java_com_sun_mmedia_DirectRecord_nCommit),
  JVM_NATIVE("nGetRecordedData","(III[B)I",              Java_com_sun_mmedia_DirectRecord_nGetRecordedData),
  JVM_NATIVE("nGetRecordedSize","(I)I",                  Java_com_sun_mmedia_DirectRecord_nGetRecordedSize),
  JVM_NATIVE("nGetRecordedType","(I)Ljava/lang/String;", Java_com_sun_mmedia_DirectRecord_nGetRecordedType),
  JVM_NATIVE("nPause",        "(I)I",                  Java_com_sun_mmedia_DirectRecord_nPause),
  JVM_NATIVE("nReset",        "(I)I",                  Java_com_sun_mmedia_DirectRecord_nReset),
  JVM_NATIVE("nSetLocator",   "(ILjava/lang/String;)I", Java_com_sun_mmedia_DirectRecord_nSetLocator),
  JVM_NATIVE("nSetSizeLimit", "(II)I",                 Java_com_sun_mmedia_DirectRecord_nSetSizeLimit),
  JVM_NATIVE("nSetSizeLimitIsSupported","(I)Z",                  Java_com_sun_mmedia_DirectRecord_nSetSizeLimitIsSupported),
  JVM_NATIVE("nStart",        "(I)I",                  Java_com_sun_mmedia_DirectRecord_nStart),
  JVM_NATIVE("nStop",         "(I)I",                  Java_com_sun_mmedia_DirectRecord_nStop),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_DirectVideo_natives[] = {
  JVM_NATIVE("nGetHeight",    "(I)I",                  Java_com_sun_mmedia_DirectVideo_nGetHeight),
  JVM_NATIVE("nGetScreenHeight","()I",                   Java_com_sun_mmedia_DirectVideo_nGetScreenHeight),
  JVM_NATIVE("nGetScreenWidth","()I",                   Java_com_sun_mmedia_DirectVideo_nGetScreenWidth),
  JVM_NATIVE("nGetWidth",     "(I)I",                  Java_com_sun_mmedia_DirectVideo_nGetWidth),
  JVM_NATIVE("nSetAlpha",     "(ZI)I",                 Java_com_sun_mmedia_DirectVideo_nSetAlpha),
  JVM_NATIVE("nSetLocation",  "(IIIII)Z",              Java_com_sun_mmedia_DirectVideo_nSetLocation),
  JVM_NATIVE("nSetVisible",   "(IZ)Z",                 Java_com_sun_mmedia_DirectVideo_nSetVisible),
  JVM_NATIVE("nSnapShot",     "(ILjava/lang/String;)[B", Java_com_sun_mmedia_DirectVideo_nSnapShot),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_DirectVolume_natives[] = {
  JVM_NATIVE("nGetVolume",    "(I)I",                  Java_com_sun_mmedia_DirectVolume_nGetVolume),
  JVM_NATIVE("nIsMuted",      "(I)Z",                  Java_com_sun_mmedia_DirectVolume_nIsMuted),
  JVM_NATIVE("nSetMute",      "(IZ)Z",                 Java_com_sun_mmedia_DirectVolume_nSetMute),
  JVM_NATIVE("nSetVolume",    "(II)I",                 Java_com_sun_mmedia_DirectVolume_nSetVolume),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_mmedia_NativeTonePlayer_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_mmedia_NativeTonePlayer_finalize),
  JVM_NATIVE("nPlayTone",     "(III)Z",                Java_com_sun_mmedia_NativeTonePlayer_nPlayTone),
  JVM_NATIVE("nStopTone",     "()Z",                   Java_com_sun_mmedia_NativeTonePlayer_nStopTone),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_AbstractSurface_natives[] = {
  JVM_NATIVE("drawRGBImpl",   "([IIIIIIIF)V",          Java_com_sun_pisces_AbstractSurface_drawRGBImpl),
  JVM_NATIVE("drawSurfaceImpl","(Lcom/sun/pisces/NativeSurface;IIIIIIF)V", Java_com_sun_pisces_AbstractSurface_drawSurfaceImpl),
  JVM_NATIVE("getHeight",     "()I",                   Java_com_sun_pisces_AbstractSurface_getHeight),
  JVM_NATIVE("getRGB",        "([IIIIIII)V",           Java_com_sun_pisces_AbstractSurface_getRGB),
  JVM_NATIVE("getWidth",      "()I",                   Java_com_sun_pisces_AbstractSurface_getWidth),
  JVM_NATIVE("nativeFinalize","()V",                   Java_com_sun_pisces_AbstractSurface_nativeFinalize),
  JVM_NATIVE("setRGB",        "([IIIIIII)V",           Java_com_sun_pisces_AbstractSurface_setRGB),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_GraphicsSurfaceDestination_natives[] = {
  JVM_NATIVE("drawRGBImpl",   "(Ljavax/microedition/lcdui/Graphics;[IIIIIIIF)V", Java_com_sun_pisces_GraphicsSurfaceDestination_drawRGBImpl),
  JVM_NATIVE("drawSurfaceImpl","(Ljavax/microedition/lcdui/Graphics;Lcom/sun/pisces/AbstractSurface;IIIIIIF)V", Java_com_sun_pisces_GraphicsSurfaceDestination_drawSurfaceImpl),
  JVM_NATIVE("initialize",    "()V",                   Java_com_sun_pisces_GraphicsSurfaceDestination_initialize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_NativeFinalizer_natives[] = {
  JVM_NATIVE("initialize",    "()V",                   Java_com_sun_pisces_NativeFinalizer_initialize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_NativeFinalizer_RendererNativeFinalizer_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_pisces_NativeFinalizer_00024RendererNativeFinalizer_finalize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_NativeFinalizer_SurfaceNativeFinalizer_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_pisces_NativeFinalizer_00024SurfaceNativeFinalizer_finalize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_NativeSurface_natives[] = {
  JVM_NATIVE("initialize",    "(III)V",                Java_com_sun_pisces_NativeSurface_initialize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_PiscesFinalizer_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_com_sun_pisces_PiscesFinalizer_finalize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_PiscesRenderer_natives[] = {
  JVM_NATIVE("beginRendering","(I)V",                  Java_com_sun_pisces_PiscesRenderer_beginRendering__I),
  JVM_NATIVE("beginRendering","(IIIII)V",              Java_com_sun_pisces_PiscesRenderer_beginRendering__IIIII),
  JVM_NATIVE("clearRect",     "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_clearRect),
  JVM_NATIVE("close",         "()V",                   Java_com_sun_pisces_PiscesRenderer_close),
  JVM_NATIVE("cubicTo",       "(IIIIII)V",             Java_com_sun_pisces_PiscesRenderer_cubicTo),
  JVM_NATIVE("drawArc",       "(IIIIIII)V",            Java_com_sun_pisces_PiscesRenderer_drawArc),
  JVM_NATIVE("drawLine",      "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_drawLine),
  JVM_NATIVE("drawOval",      "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_drawOval),
  JVM_NATIVE("drawRect",      "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_drawRect),
  JVM_NATIVE("drawRoundRect", "(IIIIII)V",             Java_com_sun_pisces_PiscesRenderer_drawRoundRect),
  JVM_NATIVE("end",           "()V",                   Java_com_sun_pisces_PiscesRenderer_end),
  JVM_NATIVE("endRendering",  "()V",                   Java_com_sun_pisces_PiscesRenderer_endRendering),
  JVM_NATIVE("fillArc",       "(IIIIIII)V",            Java_com_sun_pisces_PiscesRenderer_fillArc),
  JVM_NATIVE("fillOval",      "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_fillOval),
  JVM_NATIVE("fillRect",      "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_fillRect),
  JVM_NATIVE("fillRoundRect", "(IIIIII)V",             Java_com_sun_pisces_PiscesRenderer_fillRoundRect),
  JVM_NATIVE("getAntialiasing","()Z",                   Java_com_sun_pisces_PiscesRenderer_getAntialiasing),
  JVM_NATIVE("getBoundingBox","([I)V",                 Java_com_sun_pisces_PiscesRenderer_getBoundingBox),
  JVM_NATIVE("getTransformImpl","(Lcom/sun/pisces/Transform6;)V", Java_com_sun_pisces_PiscesRenderer_getTransformImpl),
  JVM_NATIVE("initialize",    "()V",                   Java_com_sun_pisces_PiscesRenderer_initialize),
  JVM_NATIVE("lineJoin",      "()V",                   Java_com_sun_pisces_PiscesRenderer_lineJoin),
  JVM_NATIVE("lineTo",        "(II)V",                 Java_com_sun_pisces_PiscesRenderer_lineTo),
  JVM_NATIVE("moveTo",        "(II)V",                 Java_com_sun_pisces_PiscesRenderer_moveTo),
  JVM_NATIVE("nativeFinalize","()V",                   Java_com_sun_pisces_PiscesRenderer_nativeFinalize),
  JVM_NATIVE("quadTo",        "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_quadTo),
  JVM_NATIVE("resetClip",     "()V",                   Java_com_sun_pisces_PiscesRenderer_resetClip),
  JVM_NATIVE("setAntialiasing","(Z)V",                  Java_com_sun_pisces_PiscesRenderer_setAntialiasing),
  JVM_NATIVE("setClip",       "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_setClip),
  JVM_NATIVE("setColor",      "(IIII)V",               Java_com_sun_pisces_PiscesRenderer_setColor),
  JVM_NATIVE("setComposite",  "(IF)V",                 Java_com_sun_pisces_PiscesRenderer_setComposite),
  JVM_NATIVE("setCompositeRule","(I)V",                  Java_com_sun_pisces_PiscesRenderer_setCompositeRule),
  JVM_NATIVE("setFill",       "()V",                   Java_com_sun_pisces_PiscesRenderer_setFill),
  JVM_NATIVE("setLinearGradientImpl","(IIII[IILcom/sun/pisces/Transform6;)V", Java_com_sun_pisces_PiscesRenderer_setLinearGradientImpl),
  JVM_NATIVE("setPathData",   "([F[BI)V",              Java_com_sun_pisces_PiscesRenderer_setPathData),
  JVM_NATIVE("setRadialGradientImpl","(IIIII[IILcom/sun/pisces/Transform6;)V", Java_com_sun_pisces_PiscesRenderer_setRadialGradientImpl),
  JVM_NATIVE("setStroke",     "()V",                   Java_com_sun_pisces_PiscesRenderer_setStroke__),
  JVM_NATIVE("setStroke",     "(IIII[II)V",            Java_com_sun_pisces_PiscesRenderer_setStroke__IIII_3II),
  JVM_NATIVE("setTextureImpl","(I[IIIIILcom/sun/pisces/Transform6;Z)V", Java_com_sun_pisces_PiscesRenderer_setTextureImpl),
  JVM_NATIVE("setTransform",  "(Lcom/sun/pisces/Transform6;)V", Java_com_sun_pisces_PiscesRenderer_setTransform),
  JVM_NATIVE("staticInitialize","(II)V",                 Java_com_sun_pisces_PiscesRenderer_staticInitialize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction com_sun_pisces_Transform6_natives[] = {
  JVM_NATIVE("initialize",    "()V",                   Java_com_sun_pisces_Transform6_initialize),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Class_natives[] = {
  JVM_NATIVE("forName",       "(Ljava/lang/String;)Ljava/lang/Class;", Java_java_lang_Class_forName),
  JVM_NATIVE("getName",       "()Ljava/lang/String;",  Java_java_lang_Class_getName),
  JVM_NATIVE("getSuperclass", "()Ljava/lang/Class;",   Java_java_lang_Class_getSuperclass),
  JVM_NATIVE("init9",         "()V",                   Java_java_lang_Class_init9),
  JVM_NATIVE("invoke_clinit", "()V",                   Java_java_lang_Class_invoke_1clinit),
  JVM_NATIVE("invoke_verify", "()V",                   Java_java_lang_Class_invoke_1verify),
  JVM_NATIVE("isArray",       "()Z",                   Java_java_lang_Class_isArray),
  JVM_NATIVE("isAssignableFrom","(Ljava/lang/Class;)Z",  Java_java_lang_Class_isAssignableFrom),
  JVM_NATIVE("isInstance",    "(Ljava/lang/Object;)Z", Java_java_lang_Class_isInstance),
  JVM_NATIVE("isInterface",   "()Z",                   Java_java_lang_Class_isInterface),
  JVM_NATIVE("newInstance",   "()Ljava/lang/Object;",  Java_java_lang_Class_newInstance),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Double_natives[] = {
  JVM_NATIVE("doubleToLongBits","(D)J",                  Java_java_lang_Double_doubleToLongBits),
  JVM_NATIVE("longBitsToDouble","(J)D",                  Java_java_lang_Double_longBitsToDouble),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Float_natives[] = {
  JVM_NATIVE("floatToIntBits","(F)I",                  Java_java_lang_Float_floatToIntBits),
  JVM_NATIVE("intBitsToFloat","(I)F",                  Java_java_lang_Float_intBitsToFloat),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Integer_entries[] = {
  JVM_ENTRY("toString",            "(I)Ljava/lang/String;", native_integer_toString_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Math_entries[] = {
  JVM_ENTRY("ceil",                "(D)D", native_math_ceil_entry),
  JVM_ENTRY("cos",                 "(D)D", native_math_cos_entry),
  JVM_ENTRY("floor",               "(D)D", native_math_floor_entry),
  JVM_ENTRY("sin",                 "(D)D", native_math_sin_entry),
  JVM_ENTRY("sqrt",                "(D)D", native_math_sqrt_entry),
  JVM_ENTRY("tan",                 "(D)D", native_math_tan_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Object_natives[] = {
  JVM_NATIVE("getClass",      "()Ljava/lang/Class;",   Java_java_lang_Object_getClass),
  JVM_NATIVE("hashCode",      "()I",                   Java_java_lang_Object_hashCode),
  JVM_NATIVE("notify",        "()V",                   Java_java_lang_Object_notify),
  JVM_NATIVE("notifyAll",     "()V",                   Java_java_lang_Object_notifyAll),
  JVM_NATIVE("wait",          "(J)V",                  Java_java_lang_Object_wait),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Runtime_natives[] = {
  JVM_NATIVE("exitInternal",  "(I)V",                  Java_java_lang_Runtime_exitInternal),
  JVM_NATIVE("freeMemory",    "()J",                   Java_java_lang_Runtime_freeMemory),
  JVM_NATIVE("gc",            "()V",                   Java_java_lang_Runtime_gc),
  JVM_NATIVE("totalMemory",   "()J",                   Java_java_lang_Runtime_totalMemory),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_String_natives[] = {
  JVM_NATIVE("hashCode",      "()I",                   Java_java_lang_String_hashCode),
  JVM_NATIVE("intern",        "()Ljava/lang/String;",  Java_java_lang_String_intern),
  JVM_NATIVE("lastIndexOf",   "(I)I",                  Java_java_lang_String_lastIndexOf__I),
  JVM_NATIVE("lastIndexOf",   "(II)I",                 Java_java_lang_String_lastIndexOf__II),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_String_entries[] = {
  JVM_ENTRY("<init>",              "(Ljava/lang/StringBuffer;)V", native_string_init_entry),
  JVM_ENTRY("charAt",              "(I)C", native_string_charAt_entry),
  JVM_ENTRY("endsWith",            "(Ljava/lang/String;)Z", native_string_endsWith_entry),
  JVM_ENTRY("equals",              "(Ljava/lang/Object;)Z", native_string_equals_entry),
  JVM_ENTRY("indexOf",             "(I)I", native_string_indexof0_entry),
  JVM_ENTRY("indexOf",             "(II)I", native_string_indexof_entry),
  JVM_ENTRY("indexOf",             "(Ljava/lang/String;)I", native_string_indexof0_string_entry),
  JVM_ENTRY("indexOf",             "(Ljava/lang/String;I)I", native_string_indexof_string_entry),
  JVM_ENTRY("startsWith",          "(Ljava/lang/String;)Z", native_string_startsWith0_entry),
  JVM_ENTRY("startsWith",          "(Ljava/lang/String;I)Z", native_string_startsWith_entry),
  JVM_ENTRY("substring",           "(I)Ljava/lang/String;", native_string_substringI_entry),
  JVM_ENTRY("substring",           "(II)Ljava/lang/String;", native_string_substringII_entry),
  JVM_ENTRY("valueOf",             "(I)Ljava/lang/String;", native_integer_toString_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_StringBuffer_entries[] = {
  JVM_ENTRY("append",              "(C)Ljava/lang/StringBuffer;", native_stringbuffer_append_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_System_natives[] = {
  JVM_NATIVE("arraycopy",     "(Ljava/lang/Object;ILjava/lang/Object;II)V", Java_java_lang_System_arraycopy),
  JVM_NATIVE("currentTimeMillis","()J",                   Java_java_lang_System_currentTimeMillis),
  JVM_NATIVE("getProperty0",  "(Ljava/lang/String;)Ljava/lang/String;", Java_java_lang_System_getProperty0),
  JVM_NATIVE("identityHashCode","(Ljava/lang/Object;)I", Java_java_lang_System_identityHashCode),
  JVM_NATIVE("quickNativeThrow","()V",                   Java_java_lang_System_quickNativeThrow),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_System_entries[] = {
  JVM_ENTRY("arraycopy",           "(Ljava/lang/Object;ILjava/lang/Object;II)V", native_system_arraycopy_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Thread_natives[] = {
  JVM_NATIVE("activeCount",   "()I",                   Java_java_lang_Thread_activeCount),
  JVM_NATIVE("currentThread", "()Ljava/lang/Thread;",  Java_java_lang_Thread_currentThread),
  JVM_NATIVE("internalExit",  "()V",                   Java_java_lang_Thread_internalExit),
  JVM_NATIVE("interrupt0",    "()V",                   Java_java_lang_Thread_interrupt0),
  JVM_NATIVE("isAlive",       "()Z",                   Java_java_lang_Thread_isAlive),
  JVM_NATIVE("setPriority0",  "(II)V",                 Java_java_lang_Thread_setPriority0),
  JVM_NATIVE("sleep",         "(J)V",                  Java_java_lang_Thread_sleep),
  JVM_NATIVE("start0",        "()V",                   Java_java_lang_Thread_start0),
  JVM_NATIVE("yield",         "()V",                   Java_java_lang_Thread_yield),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_Throwable_natives[] = {
  JVM_NATIVE("fillInStackTrace","()V",                   Java_java_lang_Throwable_fillInStackTrace),
  JVM_NATIVE("printStackTrace","()V",                   Java_java_lang_Throwable_printStackTrace),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_lang_ref_WeakReference_natives[] = {
  JVM_NATIVE("clear",         "()V",                   Java_java_lang_ref_WeakReference_clear),
  JVM_NATIVE("finalize",      "()V",                   Java_java_lang_ref_WeakReference_finalize),
  JVM_NATIVE("get",           "()Ljava/lang/Object;",  Java_java_lang_ref_WeakReference_get),
  JVM_NATIVE("initializeWeakReference","(Ljava/lang/Object;)V", Java_java_lang_ref_WeakReference_initializeWeakReference),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction java_util_Vector_entries[] = {
  JVM_ENTRY("addElement",          "(Ljava/lang/Object;)V", native_vector_addElement_entry),
  JVM_ENTRY("elementAt",           "(I)Ljava/lang/Object;", native_vector_elementAt_entry),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_io_file_FileSystemEventHandlerBase_natives[] = {
  JVM_NATIVE("finalize",      "()V",                   Java_javax_microedition_io_file_FileSystemEventHandlerBase_finalize),
  JVM_NATIVE("registerListener","()V",                   Java_javax_microedition_io_file_FileSystemEventHandlerBase_registerListener),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_Clipboard_natives[] = {
  JVM_NATIVE("get",           "()Ljava/lang/String;",  Java_javax_microedition_lcdui_Clipboard_get),
  JVM_NATIVE("set",           "(Ljava/lang/String;)V", Java_javax_microedition_lcdui_Clipboard_set),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_Display_natives[] = {
  JVM_NATIVE("drawTrustedIcon0","(IZ)V",                 Java_javax_microedition_lcdui_Display_drawTrustedIcon0),
  JVM_NATIVE("gainedForeground0","(I)V",                  Java_javax_microedition_lcdui_Display_gainedForeground0),
  JVM_NATIVE("getReverseOrientation0","()Z",                   Java_javax_microedition_lcdui_Display_getReverseOrientation0),
  JVM_NATIVE("getScreenHeight0","()I",                   Java_javax_microedition_lcdui_Display_getScreenHeight0),
  JVM_NATIVE("getScreenWidth0","()I",                   Java_javax_microedition_lcdui_Display_getScreenWidth0),
  JVM_NATIVE("refresh0",      "(IIIII)V",              Java_javax_microedition_lcdui_Display_refresh0),
  JVM_NATIVE("reverseOrientation0","()Z",                   Java_javax_microedition_lcdui_Display_reverseOrientation0),
  JVM_NATIVE("setFullScreen0","(IZ)V",                 Java_javax_microedition_lcdui_Display_setFullScreen0),
  JVM_NATIVE("vibrate0",      "(II)Z",                 Java_javax_microedition_lcdui_Display_vibrate0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_Font_natives[] = {
  JVM_NATIVE("charWidth",     "(C)I",                  Java_javax_microedition_lcdui_Font_charWidth),
  JVM_NATIVE("charsWidth",    "([CII)I",               Java_javax_microedition_lcdui_Font_charsWidth),
  JVM_NATIVE("init",          "(III)V",                Java_javax_microedition_lcdui_Font_init),
  JVM_NATIVE("stringWidth",   "(Ljava/lang/String;)I", Java_javax_microedition_lcdui_Font_stringWidth),
  JVM_NATIVE("substringWidth","(Ljava/lang/String;II)I", Java_javax_microedition_lcdui_Font_substringWidth),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_Graphics_natives[] = {
  JVM_NATIVE("doCopyArea",    "(IIIIIII)V",            Java_javax_microedition_lcdui_Graphics_doCopyArea),
  JVM_NATIVE("drawArc",       "(IIIIII)V",             Java_javax_microedition_lcdui_Graphics_drawArc),
  JVM_NATIVE("drawChar",      "(CIII)V",               Java_javax_microedition_lcdui_Graphics_drawChar),
  JVM_NATIVE("drawChars",     "([CIIIII)V",            Java_javax_microedition_lcdui_Graphics_drawChars),
  JVM_NATIVE("drawLine",      "(IIII)V",               Java_javax_microedition_lcdui_Graphics_drawLine),
  JVM_NATIVE("drawRGB",       "([IIIIIIIZ)V",          Java_javax_microedition_lcdui_Graphics_drawRGB),
  JVM_NATIVE("drawRect",      "(IIII)V",               Java_javax_microedition_lcdui_Graphics_drawRect),
  JVM_NATIVE("drawRoundRect", "(IIIIII)V",             Java_javax_microedition_lcdui_Graphics_drawRoundRect),
  JVM_NATIVE("drawString",    "(Ljava/lang/String;III)V", Java_javax_microedition_lcdui_Graphics_drawString),
  JVM_NATIVE("drawSubstring", "(Ljava/lang/String;IIIII)V", Java_javax_microedition_lcdui_Graphics_drawSubstring),
  JVM_NATIVE("drawUtilityString","(Ljava/lang/String;III)V", Java_javax_microedition_lcdui_Graphics_drawUtilityString),
  JVM_NATIVE("fillArc",       "(IIIIII)V",             Java_javax_microedition_lcdui_Graphics_fillArc),
  JVM_NATIVE("fillRect",      "(IIII)V",               Java_javax_microedition_lcdui_Graphics_fillRect),
  JVM_NATIVE("fillRoundRect", "(IIIIII)V",             Java_javax_microedition_lcdui_Graphics_fillRoundRect),
  JVM_NATIVE("fillTriangle",  "(IIIIII)V",             Java_javax_microedition_lcdui_Graphics_fillTriangle),
  JVM_NATIVE("getDisplayColor","(I)I",                  Java_javax_microedition_lcdui_Graphics_getDisplayColor),
  JVM_NATIVE("getPixel",      "(IIZ)I",                Java_javax_microedition_lcdui_Graphics_getPixel),
  JVM_NATIVE("render",        "(Ljavax/microedition/lcdui/Image;III)Z", Java_javax_microedition_lcdui_Graphics_render),
  JVM_NATIVE("renderRegion",  "(Ljavax/microedition/lcdui/Image;IIIIIIII)Z", Java_javax_microedition_lcdui_Graphics_renderRegion),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_ImageData_natives[] = {
  JVM_NATIVE("getRGB",        "([IIIIIII)V",           Java_javax_microedition_lcdui_ImageData_getRGB),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_ImageDataFactory_natives[] = {
  JVM_NATIVE("loadCachedImage0","(Ljavax/microedition/lcdui/ImageData;ILjava/lang/String;)Z", Java_javax_microedition_lcdui_ImageDataFactory_loadCachedImage0),
  JVM_NATIVE("loadGIF",       "(Ljavax/microedition/lcdui/ImageData;[BII)Z", Java_javax_microedition_lcdui_ImageDataFactory_loadGIF),
  JVM_NATIVE("loadJPEG",      "(Ljavax/microedition/lcdui/ImageData;[BII)V", Java_javax_microedition_lcdui_ImageDataFactory_loadJPEG),
  JVM_NATIVE("loadPNG",       "(Ljavax/microedition/lcdui/ImageData;[BII)Z", Java_javax_microedition_lcdui_ImageDataFactory_loadPNG),
  JVM_NATIVE("loadRAW",       "(Ljavax/microedition/lcdui/ImageData;[BII)V", Java_javax_microedition_lcdui_ImageDataFactory_loadRAW),
  JVM_NATIVE("loadRGB",       "(Ljavax/microedition/lcdui/ImageData;[I)V", Java_javax_microedition_lcdui_ImageDataFactory_loadRGB),
  JVM_NATIVE("loadRegion",    "(Ljavax/microedition/lcdui/ImageData;Ljavax/microedition/lcdui/ImageData;IIIII)V", Java_javax_microedition_lcdui_ImageDataFactory_loadRegion),
  JVM_NATIVE("loadRomizedImage","(Ljavax/microedition/lcdui/ImageData;II)Z", Java_javax_microedition_lcdui_ImageDataFactory_loadRomizedImage),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_KeyConverter_natives[] = {
  JVM_NATIVE("getGameAction", "(I)I",                  Java_javax_microedition_lcdui_KeyConverter_getGameAction),
  JVM_NATIVE("getKeyCode",    "(I)I",                  Java_javax_microedition_lcdui_KeyConverter_getKeyCode),
  JVM_NATIVE("getKeyName",    "(I)Ljava/lang/String;", Java_javax_microedition_lcdui_KeyConverter_getKeyName),
  JVM_NATIVE("getSystemKey",  "(I)I",                  Java_javax_microedition_lcdui_KeyConverter_getSystemKey),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_TextFieldLFImpl_natives[] = {
  JVM_NATIVE("launchNativeTextField0","(ILcom/sun/midp/lcdui/DynamicCharacterArray;Ljava/lang/String;)I", Java_javax_microedition_lcdui_TextFieldLFImpl_launchNativeTextField0),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_lcdui_game_GameCanvas_natives[] = {
  JVM_NATIVE("setSuppressKeyEvents","(Ljavax/microedition/lcdui/Canvas;Z)V", Java_javax_microedition_lcdui_game_GameCanvas_setSuppressKeyEvents),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_location_Coordinates_natives[] = {
  JVM_NATIVE("atan2",         "(DD)D",                 Java_javax_microedition_location_Coordinates_atan2),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_AnimationController_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_AnimationController_nCreate),
  JVM_NATIVE("nSetActiveInterval","(III)V",                Java_javax_microedition_m3g_AnimationController_nSetActiveInterval),
  JVM_NATIVE("nSetPosition",  "(IFI)V",                Java_javax_microedition_m3g_AnimationController_nSetPosition),
  JVM_NATIVE("nSetSpeed",     "(IFI)V",                Java_javax_microedition_m3g_AnimationController_nSetSpeed),
  JVM_NATIVE("nSetWeight",    "(IF)V",                 Java_javax_microedition_m3g_AnimationController_nSetWeight),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_AnimationTrack_natives[] = {
  JVM_NATIVE("nCreate",       "(II)I",                 Java_javax_microedition_m3g_AnimationTrack_nCreate),
  JVM_NATIVE("nGetSequence",  "(I)I",                  Java_javax_microedition_m3g_AnimationTrack_nGetSequence),
  JVM_NATIVE("nGetTargetProperty","(I)I",                  Java_javax_microedition_m3g_AnimationTrack_nGetTargetProperty),
  JVM_NATIVE("nSetController","(II)V",                 Java_javax_microedition_m3g_AnimationTrack_nSetController),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Appearance_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Appearance_nCreate),
  JVM_NATIVE("nGetCompositingMode","(I)I",                  Java_javax_microedition_m3g_Appearance_nGetCompositingMode),
  JVM_NATIVE("nGetFog",       "(I)I",                  Java_javax_microedition_m3g_Appearance_nGetFog),
  JVM_NATIVE("nGetLayer",     "(I)I",                  Java_javax_microedition_m3g_Appearance_nGetLayer),
  JVM_NATIVE("nGetMaterial",  "(I)I",                  Java_javax_microedition_m3g_Appearance_nGetMaterial),
  JVM_NATIVE("nGetPolygonMode","(I)I",                  Java_javax_microedition_m3g_Appearance_nGetPolygonMode),
  JVM_NATIVE("nGetTexture",   "(II)I",                 Java_javax_microedition_m3g_Appearance_nGetTexture),
  JVM_NATIVE("nSetCompositingMode","(II)V",                 Java_javax_microedition_m3g_Appearance_nSetCompositingMode),
  JVM_NATIVE("nSetFog",       "(II)V",                 Java_javax_microedition_m3g_Appearance_nSetFog),
  JVM_NATIVE("nSetLayer",     "(II)V",                 Java_javax_microedition_m3g_Appearance_nSetLayer),
  JVM_NATIVE("nSetMaterial",  "(II)V",                 Java_javax_microedition_m3g_Appearance_nSetMaterial),
  JVM_NATIVE("nSetPolygonMode","(II)V",                 Java_javax_microedition_m3g_Appearance_nSetPolygonMode),
  JVM_NATIVE("nSetTexture",   "(III)V",                Java_javax_microedition_m3g_Appearance_nSetTexture),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Background_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Background_nCreate),
  JVM_NATIVE("nSetColor",     "(II)V",                 Java_javax_microedition_m3g_Background_nSetColor),
  JVM_NATIVE("nSetCrop",      "(IIIII)V",              Java_javax_microedition_m3g_Background_nSetCrop),
  JVM_NATIVE("nSetEnable",    "(III)V",                Java_javax_microedition_m3g_Background_nSetEnable),
  JVM_NATIVE("nSetImage",     "(II)V",                 Java_javax_microedition_m3g_Background_nSetImage),
  JVM_NATIVE("nSetImageMode", "(III)V",                Java_javax_microedition_m3g_Background_nSetImageMode),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Camera_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Camera_nCreate),
  JVM_NATIVE("nSetGeneric",   "(I[F)V",                Java_javax_microedition_m3g_Camera_nSetGeneric),
  JVM_NATIVE("nSetParallel",  "(IFFFF)V",              Java_javax_microedition_m3g_Camera_nSetParallel),
  JVM_NATIVE("nSetPerspective","(IFFFF)V",              Java_javax_microedition_m3g_Camera_nSetPerspective),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_CompositingMode_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_CompositingMode_nCreate),
  JVM_NATIVE("nEnableAlphaWrite","(II)V",                 Java_javax_microedition_m3g_CompositingMode_nEnableAlphaWrite),
  JVM_NATIVE("nEnableColorWrite","(II)V",                 Java_javax_microedition_m3g_CompositingMode_nEnableColorWrite),
  JVM_NATIVE("nEnableDepthTest","(II)V",                 Java_javax_microedition_m3g_CompositingMode_nEnableDepthTest),
  JVM_NATIVE("nEnableDepthWrite","(II)V",                 Java_javax_microedition_m3g_CompositingMode_nEnableDepthWrite),
  JVM_NATIVE("nSetAlphaThreshold","(IF)V",                 Java_javax_microedition_m3g_CompositingMode_nSetAlphaThreshold),
  JVM_NATIVE("nSetBlending",  "(II)V",                 Java_javax_microedition_m3g_CompositingMode_nSetBlending),
  JVM_NATIVE("nSetDepthOffset","(IFF)V",                Java_javax_microedition_m3g_CompositingMode_nSetDepthOffset),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Fog_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Fog_nCreate),
  JVM_NATIVE("nSetColor",     "(II)V",                 Java_javax_microedition_m3g_Fog_nSetColor),
  JVM_NATIVE("nSetDensity",   "(IF)V",                 Java_javax_microedition_m3g_Fog_nSetDensity),
  JVM_NATIVE("nSetLinear",    "(IFF)V",                Java_javax_microedition_m3g_Fog_nSetLinear),
  JVM_NATIVE("nSetMode",      "(II)V",                 Java_javax_microedition_m3g_Fog_nSetMode),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Graphics3D_natives[] = {
  JVM_NATIVE("nAddLight",     "(I[F)I",                Java_javax_microedition_m3g_Graphics3D_nAddLight),
  JVM_NATIVE("nBind",         "(Ljava/lang/Object;II)I", Java_javax_microedition_m3g_Graphics3D_nBind),
  JVM_NATIVE("nClear",        "(I)I",                  Java_javax_microedition_m3g_Graphics3D_nClear),
  JVM_NATIVE("nClearLights",  "()V",                   Java_javax_microedition_m3g_Graphics3D_nClearLights),
  JVM_NATIVE("nRelease",      "(Ljava/lang/Object;)I", Java_javax_microedition_m3g_Graphics3D_nRelease),
  JVM_NATIVE("nRenderImmediate","(III[FI)I",             Java_javax_microedition_m3g_Graphics3D_nRenderImmediate),
  JVM_NATIVE("nRenderNode",   "(I[F)I",                Java_javax_microedition_m3g_Graphics3D_nRenderNode),
  JVM_NATIVE("nRenderWorld",  "(I)I",                  Java_javax_microedition_m3g_Graphics3D_nRenderWorld),
  JVM_NATIVE("nSetCamera",    "(I[F)I",                Java_javax_microedition_m3g_Graphics3D_nSetCamera),
  JVM_NATIVE("nSetClipRect",  "(IIII)V",               Java_javax_microedition_m3g_Graphics3D_nSetClipRect),
  JVM_NATIVE("nSetDepthRange","(FF)V",                 Java_javax_microedition_m3g_Graphics3D_nSetDepthRange),
  JVM_NATIVE("nSetViewport",  "(IIII)V",               Java_javax_microedition_m3g_Graphics3D_nSetViewport),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Group_natives[] = {
  JVM_NATIVE("nAddChild",     "(II)V",                 Java_javax_microedition_m3g_Group_nAddChild),
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Group_nCreate),
  JVM_NATIVE("nGetChild",     "(II)I",                 Java_javax_microedition_m3g_Group_nGetChild),
  JVM_NATIVE("nGetChildCount","(I)I",                  Java_javax_microedition_m3g_Group_nGetChildCount),
  JVM_NATIVE("nRemoveChild",  "(II)V",                 Java_javax_microedition_m3g_Group_nRemoveChild),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Image2D_natives[] = {
  JVM_NATIVE("nCommit",       "(I)V",                  Java_javax_microedition_m3g_Image2D_nCommit),
  JVM_NATIVE("nCreate",       "(IIII)I",               Java_javax_microedition_m3g_Image2D_nCreate),
  JVM_NATIVE("nGetFormat",    "(I)I",                  Java_javax_microedition_m3g_Image2D_nGetFormat),
  JVM_NATIVE("nGetHeight",    "(I)I",                  Java_javax_microedition_m3g_Image2D_nGetHeight),
  JVM_NATIVE("nGetWidth",     "(I)I",                  Java_javax_microedition_m3g_Image2D_nGetWidth),
  JVM_NATIVE("nSetImage",     "(I[B)V",                Java_javax_microedition_m3g_Image2D_nSetImage),
  JVM_NATIVE("nSetPalette",   "(II[B)V",               Java_javax_microedition_m3g_Image2D_nSetPalette),
  JVM_NATIVE("nSetSubImage",  "(IIIII[B)V",            Java_javax_microedition_m3g_Image2D_nSetSubImage),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_KeyframeSequence_natives[] = {
  JVM_NATIVE("nCreate",       "(III)I",                Java_javax_microedition_m3g_KeyframeSequence_nCreate),
  JVM_NATIVE("nGetDuration",  "(I)I",                  Java_javax_microedition_m3g_KeyframeSequence_nGetDuration),
  JVM_NATIVE("nGetKeyframeCount","(I)I",                  Java_javax_microedition_m3g_KeyframeSequence_nGetKeyframeCount),
  JVM_NATIVE("nSetDuration",  "(II)V",                 Java_javax_microedition_m3g_KeyframeSequence_nSetDuration),
  JVM_NATIVE("nSetKeyframe",  "(IIII[F)V",             Java_javax_microedition_m3g_KeyframeSequence_nSetKeyframe),
  JVM_NATIVE("nSetRepeatMode","(II)V",                 Java_javax_microedition_m3g_KeyframeSequence_nSetRepeatMode),
  JVM_NATIVE("nSetValidRange","(III)V",                Java_javax_microedition_m3g_KeyframeSequence_nSetValidRange),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Light_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Light_nCreate),
  JVM_NATIVE("nSetAttenuation","(IFFF)V",               Java_javax_microedition_m3g_Light_nSetAttenuation),
  JVM_NATIVE("nSetColor",     "(II)V",                 Java_javax_microedition_m3g_Light_nSetColor),
  JVM_NATIVE("nSetIntensity", "(IF)V",                 Java_javax_microedition_m3g_Light_nSetIntensity),
  JVM_NATIVE("nSetMode",      "(II)V",                 Java_javax_microedition_m3g_Light_nSetMode),
  JVM_NATIVE("nSetSpotAngle", "(IF)V",                 Java_javax_microedition_m3g_Light_nSetSpotAngle),
  JVM_NATIVE("nSetSpotExponent","(IF)V",                 Java_javax_microedition_m3g_Light_nSetSpotExponent),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Loader_natives[] = {
  JVM_NATIVE("nLoadData",     "([BII)I",               Java_javax_microedition_m3g_Loader_nLoadData),
  JVM_NATIVE("nResultAbort",  "()V",                   Java_javax_microedition_m3g_Loader_nResultAbort),
  JVM_NATIVE("nResultClass",  "(I)I",                  Java_javax_microedition_m3g_Loader_nResultClass),
  JVM_NATIVE("nResultCommit", "()V",                   Java_javax_microedition_m3g_Loader_nResultCommit),
  JVM_NATIVE("nResultHandle", "(I)I",                  Java_javax_microedition_m3g_Loader_nResultHandle),
  JVM_NATIVE("nUserObjectCount","()I",                   Java_javax_microedition_m3g_Loader_nUserObjectCount),
  JVM_NATIVE("nUserObjectHandle","(I)I",                  Java_javax_microedition_m3g_Loader_nUserObjectHandle),
  JVM_NATIVE("nUserParam",    "(II[B)I",               Java_javax_microedition_m3g_Loader_nUserParam),
  JVM_NATIVE("nUserParamCount","(I)I",                  Java_javax_microedition_m3g_Loader_nUserParamCount),
  JVM_NATIVE("nUserParamLength","(II)I",                 Java_javax_microedition_m3g_Loader_nUserParamLength),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Material_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_Material_nCreate),
  JVM_NATIVE("nGetColor",     "(II)I",                 Java_javax_microedition_m3g_Material_nGetColor),
  JVM_NATIVE("nSetColor",     "(III)V",                Java_javax_microedition_m3g_Material_nSetColor),
  JVM_NATIVE("nSetShininess", "(IF)V",                 Java_javax_microedition_m3g_Material_nSetShininess),
  JVM_NATIVE("nSetVertexColorTracking","(II)V",                 Java_javax_microedition_m3g_Material_nSetVertexColorTracking),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Mesh_natives[] = {
  JVM_NATIVE("nCreate",       "(I[I[I)I",              Java_javax_microedition_m3g_Mesh_nCreate),
  JVM_NATIVE("nGetAppearance","(II)I",                 Java_javax_microedition_m3g_Mesh_nGetAppearance),
  JVM_NATIVE("nGetIndexBuffer","(II)I",                 Java_javax_microedition_m3g_Mesh_nGetIndexBuffer),
  JVM_NATIVE("nGetSubmeshCount","(I)I",                  Java_javax_microedition_m3g_Mesh_nGetSubmeshCount),
  JVM_NATIVE("nGetVertexBuffer","(I)I",                  Java_javax_microedition_m3g_Mesh_nGetVertexBuffer),
  JVM_NATIVE("nSetAppearance","(III)V",                Java_javax_microedition_m3g_Mesh_nSetAppearance),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_MorphingMesh_natives[] = {
  JVM_NATIVE("nCreate",       "(I[I[I[I)I",            Java_javax_microedition_m3g_MorphingMesh_nCreate),
  JVM_NATIVE("nGetMorphTarget","(II)I",                 Java_javax_microedition_m3g_MorphingMesh_nGetMorphTarget),
  JVM_NATIVE("nGetMorphTargetCount","(I)I",                  Java_javax_microedition_m3g_MorphingMesh_nGetMorphTargetCount),
  JVM_NATIVE("nGetWeights",   "(I[FI)V",               Java_javax_microedition_m3g_MorphingMesh_nGetWeights),
  JVM_NATIVE("nSetWeights",   "(I[FI)V",               Java_javax_microedition_m3g_MorphingMesh_nSetWeights),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Node_natives[] = {
  JVM_NATIVE("nAlign",        "(II)V",                 Java_javax_microedition_m3g_Node_nAlign),
  JVM_NATIVE("nEnable",       "(III)V",                Java_javax_microedition_m3g_Node_nEnable),
  JVM_NATIVE("nGetParent",    "(I)I",                  Java_javax_microedition_m3g_Node_nGetParent),
  JVM_NATIVE("nGetTransformTo","(II[F)Z",               Java_javax_microedition_m3g_Node_nGetTransformTo),
  JVM_NATIVE("nSetAlignment", "(IIIII)V",              Java_javax_microedition_m3g_Node_nSetAlignment),
  JVM_NATIVE("nSetAlphaFactor","(IF)V",                 Java_javax_microedition_m3g_Node_nSetAlphaFactor),
  JVM_NATIVE("nSetScope",     "(II)V",                 Java_javax_microedition_m3g_Node_nSetScope),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Object3D_natives[] = {
  JVM_NATIVE("nAddAnimationTrack","(II)V",                 Java_javax_microedition_m3g_Object3D_nAddAnimationTrack),
  JVM_NATIVE("nAddRef",       "(I)V",                  Java_javax_microedition_m3g_Object3D_nAddRef),
  JVM_NATIVE("nAnimate",      "(II)I",                 Java_javax_microedition_m3g_Object3D_nAnimate),
  JVM_NATIVE("nClassID",      "(I)I",                  Java_javax_microedition_m3g_Object3D_nClassID),
  JVM_NATIVE("nDeleteRef",    "(I)V",                  Java_javax_microedition_m3g_Object3D_nDeleteRef),
  JVM_NATIVE("nDiag",         "(II)V",                 Java_javax_microedition_m3g_Object3D_nDiag),
  JVM_NATIVE("nDuplicate",    "(I)I",                  Java_javax_microedition_m3g_Object3D_nDuplicate),
  JVM_NATIVE("nFind",         "(II)I",                 Java_javax_microedition_m3g_Object3D_nFind),
  JVM_NATIVE("nGetAnimationTrack","(II)I",                 Java_javax_microedition_m3g_Object3D_nGetAnimationTrack),
  JVM_NATIVE("nGetAnimationTrackCount","(I)I",                  Java_javax_microedition_m3g_Object3D_nGetAnimationTrackCount),
  JVM_NATIVE("nGetUserID",    "(I)I",                  Java_javax_microedition_m3g_Object3D_nGetUserID),
  JVM_NATIVE("nRemoveAnimationTrack","(II)V",                 Java_javax_microedition_m3g_Object3D_nRemoveAnimationTrack),
  JVM_NATIVE("nSetUserID",    "(II)V",                 Java_javax_microedition_m3g_Object3D_nSetUserID),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_PolygonMode_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_PolygonMode_nCreate),
  JVM_NATIVE("nSetCulling",   "(II)V",                 Java_javax_microedition_m3g_PolygonMode_nSetCulling),
  JVM_NATIVE("nSetLocalCameraLighting","(II)V",                 Java_javax_microedition_m3g_PolygonMode_nSetLocalCameraLighting),
  JVM_NATIVE("nSetPerspectiveCorrection","(II)V",                 Java_javax_microedition_m3g_PolygonMode_nSetPerspectiveCorrection),
  JVM_NATIVE("nSetShading",   "(II)V",                 Java_javax_microedition_m3g_PolygonMode_nSetShading),
  JVM_NATIVE("nSetTwoSidedLighting","(II)V",                 Java_javax_microedition_m3g_PolygonMode_nSetTwoSidedLighting),
  JVM_NATIVE("nSetWinding",   "(II)V",                 Java_javax_microedition_m3g_PolygonMode_nSetWinding),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_SkinnedMesh_natives[] = {
  JVM_NATIVE("nAddTransform", "(IIIII)V",              Java_javax_microedition_m3g_SkinnedMesh_nAddTransform),
  JVM_NATIVE("nCreate",       "(I[I[II)I",             Java_javax_microedition_m3g_SkinnedMesh_nCreate),
  JVM_NATIVE("nGetSkeleton",  "(I)I",                  Java_javax_microedition_m3g_SkinnedMesh_nGetSkeleton),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Sprite3D_natives[] = {
  JVM_NATIVE("nCreate",       "(III)I",                Java_javax_microedition_m3g_Sprite3D_nCreate),
  JVM_NATIVE("nSetAppearance","(II)V",                 Java_javax_microedition_m3g_Sprite3D_nSetAppearance),
  JVM_NATIVE("nSetCrop",      "(IIIII)V",              Java_javax_microedition_m3g_Sprite3D_nSetCrop),
  JVM_NATIVE("nSetImage",     "(II)V",                 Java_javax_microedition_m3g_Sprite3D_nSetImage),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Texture2D_natives[] = {
  JVM_NATIVE("nCreate",       "(I)I",                  Java_javax_microedition_m3g_Texture2D_nCreate),
  JVM_NATIVE("nGetImage",     "(I)I",                  Java_javax_microedition_m3g_Texture2D_nGetImage),
  JVM_NATIVE("nSetBlendColor","(II)V",                 Java_javax_microedition_m3g_Texture2D_nSetBlendColor),
  JVM_NATIVE("nSetBlending",  "(II)V",                 Java_javax_microedition_m3g_Texture2D_nSetBlending),
  JVM_NATIVE("nSetFiltering", "(III)V",                Java_javax_microedition_m3g_Texture2D_nSetFiltering),
  JVM_NATIVE("nSetImage",     "(II)V",                 Java_javax_microedition_m3g_Texture2D_nSetImage),
  JVM_NATIVE("nSetWrapping",  "(III)V",                Java_javax_microedition_m3g_Texture2D_nSetWrapping),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_Transformable_natives[] = {
  JVM_NATIVE("nGetCompositeTransform","(I[F)V",                Java_javax_microedition_m3g_Transformable_nGetCompositeTransform),
  JVM_NATIVE("nGetOrientation","(I[F)V",                Java_javax_microedition_m3g_Transformable_nGetOrientation),
  JVM_NATIVE("nGetScale",     "(I[F)V",                Java_javax_microedition_m3g_Transformable_nGetScale),
  JVM_NATIVE("nGetTransform", "(I[F)V",                Java_javax_microedition_m3g_Transformable_nGetTransform),
  JVM_NATIVE("nGetTranslation","(I[F)V",                Java_javax_microedition_m3g_Transformable_nGetTranslation),
  JVM_NATIVE("nPostRotate",   "(IFFFF)V",              Java_javax_microedition_m3g_Transformable_nPostRotate),
  JVM_NATIVE("nPreRotate",    "(IFFFF)V",              Java_javax_microedition_m3g_Transformable_nPreRotate),
  JVM_NATIVE("nScale",        "(IFFF)V",               Java_javax_microedition_m3g_Transformable_nScale),
  JVM_NATIVE("nSetOrientation","(IFFFF)V",              Java_javax_microedition_m3g_Transformable_nSetOrientation),
  JVM_NATIVE("nSetScale",     "(IFFF)V",               Java_javax_microedition_m3g_Transformable_nSetScale),
  JVM_NATIVE("nSetTransform", "(I[F)V",                Java_javax_microedition_m3g_Transformable_nSetTransform),
  JVM_NATIVE("nSetTranslation","(IFFF)V",               Java_javax_microedition_m3g_Transformable_nSetTranslation),
  JVM_NATIVE("nTranslate",    "(IFFF)V",               Java_javax_microedition_m3g_Transformable_nTranslate),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_TriangleStripArray_natives[] = {
  JVM_NATIVE("nCreateExplicit","([I[I)I",               Java_javax_microedition_m3g_TriangleStripArray_nCreateExplicit),
  JVM_NATIVE("nCreateImplicit","(I[I)I",                Java_javax_microedition_m3g_TriangleStripArray_nCreateImplicit),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_VertexArray_natives[] = {
  JVM_NATIVE("nCreate",       "(III)I",                Java_javax_microedition_m3g_VertexArray_nCreate),
  JVM_NATIVE("nSetByte",      "(III[B)V",              Java_javax_microedition_m3g_VertexArray_nSetByte),
  JVM_NATIVE("nSetShort",     "(III[S)V",              Java_javax_microedition_m3g_VertexArray_nSetShort),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_VertexBuffer_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_VertexBuffer_nCreate),
  JVM_NATIVE("nGetVertexCount","(I)I",                  Java_javax_microedition_m3g_VertexBuffer_nGetVertexCount),
  JVM_NATIVE("nSetColors",    "(II)V",                 Java_javax_microedition_m3g_VertexBuffer_nSetColors),
  JVM_NATIVE("nSetDefaultColor","(II)V",                 Java_javax_microedition_m3g_VertexBuffer_nSetDefaultColor),
  JVM_NATIVE("nSetNormals",   "(II)V",                 Java_javax_microedition_m3g_VertexBuffer_nSetNormals),
  JVM_NATIVE("nSetPositions", "(IIF[F)V",              Java_javax_microedition_m3g_VertexBuffer_nSetPositions),
  JVM_NATIVE("nSetTexCoords", "(IIIF[F)V",             Java_javax_microedition_m3g_VertexBuffer_nSetTexCoords),
  {(char*)0, (char*)0, (void*)0}
};

static const JvmNativeFunction javax_microedition_m3g_World_natives[] = {
  JVM_NATIVE("nCreate",       "()I",                   Java_javax_microedition_m3g_World_nCreate),
  JVM_NATIVE("nGetActiveCamera","(I)I",                  Java_javax_microedition_m3g_World_nGetActiveCamera),
  JVM_NATIVE("nGetBackground","(I)I",                  Java_javax_microedition_m3g_World_nGetBackground),
  JVM_NATIVE("nSetActiveCamera","(II)V",                 Java_javax_microedition_m3g_World_nSetActiveCamera),
  JVM_NATIVE("nSetBackground","(II)V",                 Java_javax_microedition_m3g_World_nSetBackground),
  {(char*)0, (char*)0, (void*)0}
};

extern "C" jint Java_com_mascotcapsule_micro3d_v3_Rasterizer_nInit(void);
extern "C" void Java_com_mascotcapsule_micro3d_v3_Rasterizer_nFillAffineTReplaceFast(void);

static const JvmNativeFunction com_mascotcapsule_micro3d_v3_Rasterizer_natives[] = {
  JVM_NATIVE("nInit", "()I",
             Java_com_mascotcapsule_micro3d_v3_Rasterizer_nInit),
  JVM_NATIVE("nFillAffineTReplaceFast",
             "([IIIIIIIIIIII[BI[IIIIII)V",
             Java_com_mascotcapsule_micro3d_v3_Rasterizer_nFillAffineTReplaceFast),
  {(char*)0, (char*)0, (void*)0}
};

const JvmNativesTable jvm_natives_table[] = {
  JVM_TABLE("com/mascotcapsule/micro3d/v3/Rasterizer",
            com_mascotcapsule_micro3d_v3_Rasterizer_natives, (JvmNativeFunction*)0),
  JVM_TABLE("com/nokia/mid/ui/DirectGraphicsImpl",
                                        com_nokia_mid_ui_DirectGraphicsImpl_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/pspkvm/keypad/KeyMapInfo",
                                        com_pspkvm_keypad_KeyMapInfo_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/pspkvm/keypad/RawState",
                                        com_pspkvm_keypad_RawState_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/pspkvm/system/Power",
                                        com_pspkvm_system_Power_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/pspkvm/system/VMSettings",
                                        com_pspkvm_system_VMSettings_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/pspkvm/system/WifiStatus",
                                        com_pspkvm_system_WifiStatus_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldc/i18n/j2me/Conv",
                                        com_sun_cldc_i18n_j2me_Conv_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldc/io/ResourceInputStream",
                                        com_sun_cldc_io_ResourceInputStream_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldc/isolate/Isolate",
                                        com_sun_cldc_isolate_Isolate_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldc/util/SemaphoreLock",
                                        com_sun_cldc_util_SemaphoreLock_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldchi/io/ConsoleOutputStream",
                                        com_sun_cldchi_io_ConsoleOutputStream_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldchi/jvm/FileDescriptor",
                                        com_sun_cldchi_jvm_FileDescriptor_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/cldchi/jvm/JVM",
                                        com_sun_cldchi_jvm_JVM_natives,
                                        com_sun_cldchi_jvm_JVM_entries),
  JVM_TABLE("com/sun/j2me/location/LocationInfo",
                                        com_sun_j2me_location_LocationInfo_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/j2me/location/LocationProviderInfo",
                                        com_sun_j2me_location_LocationProviderInfo_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/j2me/location/PlatformLocationProvider",
                                        com_sun_j2me_location_PlatformLocationProvider_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/appmanager/WifiSelector",
                                        com_sun_midp_appmanager_WifiSelector_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/chameleon/input/InputModeFactory",
                                        com_sun_midp_chameleon_input_InputModeFactory_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/chameleon/input/NativeInputMode",
                                        com_sun_midp_chameleon_input_NativeInputMode_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/chameleon/skins/resources/LoadedSkinData",
                                        com_sun_midp_chameleon_skins_resources_LoadedSkinData_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/chameleon/skins/resources/LoadedSkinResources",
                                        com_sun_midp_chameleon_skins_resources_LoadedSkinResources_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/chameleon/skins/resources/SkinResources",
                                        com_sun_midp_chameleon_skins_resources_SkinResources_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/crypto/ARC4",
                                        com_sun_midp_crypto_ARC4_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/crypto/MD2",
                                        com_sun_midp_crypto_MD2_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/crypto/MD5",
                                        com_sun_midp_crypto_MD5_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/crypto/RSA",
                                        com_sun_midp_crypto_RSA_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/crypto/SHA",
                                        com_sun_midp_crypto_SHA_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/events/EventQueue",
                                        com_sun_midp_events_EventQueue_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/events/NativeEventMonitor",
                                        com_sun_midp_events_NativeEventMonitor_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/installer/DeviceDesc",
                                        com_sun_midp_installer_DeviceDesc_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/installer/OtaNotifier",
                                        com_sun_midp_installer_OtaNotifier_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/installer/SuiteDownloadInfo",
                                        com_sun_midp_installer_SuiteDownloadInfo_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/NetworkConnectionBase",
                                        com_sun_midp_io_NetworkConnectionBase_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/comm/Protocol",
                                        com_sun_midp_io_j2me_comm_Protocol_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/datagram/Protocol",
                                        com_sun_midp_io_j2me_datagram_Protocol_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/file/DefaultFileHandler",
                                        com_sun_midp_io_j2me_file_DefaultFileHandler_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/push/ConnectionRegistry",
                                        com_sun_midp_io_j2me_push_ConnectionRegistry_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/socket/Protocol",
                                        com_sun_midp_io_j2me_socket_Protocol_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/storage/File",
                                        com_sun_midp_io_j2me_storage_File_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/io/j2me/storage/RandomAccessStream",
                                        com_sun_midp_io_j2me_storage_RandomAccessStream_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/jarutil/JarReader",
                                        com_sun_midp_jarutil_JarReader_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/jsr075/Initializer",
                                        com_sun_midp_jsr075_Initializer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/l10n/LocalizedStringsBase",
                                        com_sun_midp_l10n_LocalizedStringsBase_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/l10n/LocalizedStringsBasezhCN",
                                        com_sun_midp_l10n_LocalizedStringsBasezhCN_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/lcdui/DisplayDeviceAccess",
                                        com_sun_midp_lcdui_DisplayDeviceAccess_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/links/Link",
                                        com_sun_midp_links_Link_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/links/LinkPortal",
                                        com_sun_midp_links_LinkPortal_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/log/LoggingBase",
                                        com_sun_midp_log_LoggingBase_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/AppIsolateMIDletSuiteLoader",
                                        com_sun_midp_main_AppIsolateMIDletSuiteLoader_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/CldcPlatformRequest",
                                        com_sun_midp_main_CldcPlatformRequest_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/CommandState",
                                        com_sun_midp_main_CommandState_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/Configuration",
                                        com_sun_midp_main_Configuration_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/IndicatorManager",
                                        com_sun_midp_main_IndicatorManager_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/MIDletAppImageGenerator",
                                        com_sun_midp_main_MIDletAppImageGenerator_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/MIDletProxyList",
                                        com_sun_midp_main_MIDletProxyList_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/MIDletSuiteUtils",
                                        com_sun_midp_main_MIDletSuiteUtils_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/main/MIDletSuiteVerifier",
                                        com_sun_midp_main_MIDletSuiteVerifier_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/midletsuite/InstallInfo",
                                        com_sun_midp_midletsuite_InstallInfo_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/midletsuite/MIDletSuiteImpl",
                                        com_sun_midp_midletsuite_MIDletSuiteImpl_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/midletsuite/MIDletSuiteStorage",
                                        com_sun_midp_midletsuite_MIDletSuiteStorage_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/midletsuite/SuiteProperties",
                                        com_sun_midp_midletsuite_SuiteProperties_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/midletsuite/SuiteSettings",
                                        com_sun_midp_midletsuite_SuiteSettings_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/rms/RecordStoreFactory",
                                        com_sun_midp_rms_RecordStoreFactory_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/rms/RecordStoreFile",
                                        com_sun_midp_rms_RecordStoreFile_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/rms/RecordStoreUtil",
                                        com_sun_midp_rms_RecordStoreUtil_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/suspend/SuspendSystem",
                                        com_sun_midp_suspend_SuspendSystem_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/suspend/SuspendSystem$MIDPSystem",
                                        com_sun_midp_suspend_SuspendSystem_MIDPSystem_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/midp/util/ResourceHandler",
                                        com_sun_midp_util_ResourceHandler_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/DefaultConfiguration",
                                        com_sun_mmedia_DefaultConfiguration_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/DirectMIDIControl",
                                        com_sun_mmedia_DirectMIDIControl_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/DirectPlayer",
                                        com_sun_mmedia_DirectPlayer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/DirectRecord",
                                        com_sun_mmedia_DirectRecord_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/DirectVideo",
                                        com_sun_mmedia_DirectVideo_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/DirectVolume",
                                        com_sun_mmedia_DirectVolume_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/mmedia/NativeTonePlayer",
                                        com_sun_mmedia_NativeTonePlayer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/AbstractSurface",
                                        com_sun_pisces_AbstractSurface_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/GraphicsSurfaceDestination",
                                        com_sun_pisces_GraphicsSurfaceDestination_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/NativeFinalizer",
                                        com_sun_pisces_NativeFinalizer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/NativeFinalizer$RendererNativeFinalizer",
                                        com_sun_pisces_NativeFinalizer_RendererNativeFinalizer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/NativeFinalizer$SurfaceNativeFinalizer",
                                        com_sun_pisces_NativeFinalizer_SurfaceNativeFinalizer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/NativeSurface",
                                        com_sun_pisces_NativeSurface_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/PiscesFinalizer",
                                        com_sun_pisces_PiscesFinalizer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/PiscesRenderer",
                                        com_sun_pisces_PiscesRenderer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("com/sun/pisces/Transform6",
                                        com_sun_pisces_Transform6_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/Class",
                                        java_lang_Class_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/Double",
                                        java_lang_Double_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/Float",
                                        java_lang_Float_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/Integer",
                                        (JvmNativeFunction*)0,
                                        java_lang_Integer_entries),
  JVM_TABLE("java/lang/Math",
                                        (JvmNativeFunction*)0,
                                        java_lang_Math_entries),
  JVM_TABLE("java/lang/Object",
                                        java_lang_Object_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/Runtime",
                                        java_lang_Runtime_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/String",
                                        java_lang_String_natives,
                                        java_lang_String_entries),
  JVM_TABLE("java/lang/StringBuffer",
                                        (JvmNativeFunction*)0,
                                        java_lang_StringBuffer_entries),
  JVM_TABLE("java/lang/System",
                                        java_lang_System_natives,
                                        java_lang_System_entries),
  JVM_TABLE("java/lang/Thread",
                                        java_lang_Thread_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/Throwable",
                                        java_lang_Throwable_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/lang/ref/WeakReference",
                                        java_lang_ref_WeakReference_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("java/util/Vector",
                                        (JvmNativeFunction*)0,
                                        java_util_Vector_entries),
  JVM_TABLE("javax/microedition/io/file/FileSystemEventHandlerBase",
                                        javax_microedition_io_file_FileSystemEventHandlerBase_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/Clipboard",
                                        javax_microedition_lcdui_Clipboard_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/Display",
                                        javax_microedition_lcdui_Display_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/Font",
                                        javax_microedition_lcdui_Font_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/Graphics",
                                        javax_microedition_lcdui_Graphics_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/ImageData",
                                        javax_microedition_lcdui_ImageData_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/ImageDataFactory",
                                        javax_microedition_lcdui_ImageDataFactory_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/KeyConverter",
                                        javax_microedition_lcdui_KeyConverter_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/TextFieldLFImpl",
                                        javax_microedition_lcdui_TextFieldLFImpl_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/lcdui/game/GameCanvas",
                                        javax_microedition_lcdui_game_GameCanvas_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/location/Coordinates",
                                        javax_microedition_location_Coordinates_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/AnimationController",
                                        javax_microedition_m3g_AnimationController_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/AnimationTrack",
                                        javax_microedition_m3g_AnimationTrack_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Appearance",
                                        javax_microedition_m3g_Appearance_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Background",
                                        javax_microedition_m3g_Background_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Camera",
                                        javax_microedition_m3g_Camera_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/CompositingMode",
                                        javax_microedition_m3g_CompositingMode_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Fog",
                                        javax_microedition_m3g_Fog_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Graphics3D",
                                        javax_microedition_m3g_Graphics3D_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Group",
                                        javax_microedition_m3g_Group_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Image2D",
                                        javax_microedition_m3g_Image2D_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/KeyframeSequence",
                                        javax_microedition_m3g_KeyframeSequence_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Light",
                                        javax_microedition_m3g_Light_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Loader",
                                        javax_microedition_m3g_Loader_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Material",
                                        javax_microedition_m3g_Material_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Mesh",
                                        javax_microedition_m3g_Mesh_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/MorphingMesh",
                                        javax_microedition_m3g_MorphingMesh_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Node",
                                        javax_microedition_m3g_Node_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Object3D",
                                        javax_microedition_m3g_Object3D_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/PolygonMode",
                                        javax_microedition_m3g_PolygonMode_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/SkinnedMesh",
                                        javax_microedition_m3g_SkinnedMesh_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Sprite3D",
                                        javax_microedition_m3g_Sprite3D_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Texture2D",
                                        javax_microedition_m3g_Texture2D_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/Transformable",
                                        javax_microedition_m3g_Transformable_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/TriangleStripArray",
                                        javax_microedition_m3g_TriangleStripArray_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/VertexArray",
                                        javax_microedition_m3g_VertexArray_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/VertexBuffer",
                                        javax_microedition_m3g_VertexBuffer_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE("javax/microedition/m3g/World",
                                        javax_microedition_m3g_World_natives,
                                        (JvmNativeFunction*)0),
  JVM_TABLE((char*)0, (JvmNativeFunction*)0, (JvmNativeFunction*)0)
};
const JvmExecutionEntry jvm_api_entries[] = {
{(unsigned char*)&native_jvm_unchecked_byte_arraycopy_entry,
(char*)"native_jvm_unchecked_byte_arraycopy_entry"},
{(unsigned char*)&native_jvm_unchecked_char_arraycopy_entry,
(char*)"native_jvm_unchecked_char_arraycopy_entry"},
{(unsigned char*)&native_jvm_unchecked_int_arraycopy_entry,
(char*)"native_jvm_unchecked_int_arraycopy_entry"},
{(unsigned char*)&native_jvm_unchecked_long_arraycopy_entry,
(char*)"native_jvm_unchecked_long_arraycopy_entry"},
{(unsigned char*)&native_jvm_unchecked_obj_arraycopy_entry,
(char*)"native_jvm_unchecked_obj_arraycopy_entry"},
{(unsigned char*)&native_integer_toString_entry,
(char*)"native_integer_toString_entry"},
{(unsigned char*)&native_math_ceil_entry,
(char*)"native_math_ceil_entry"},
{(unsigned char*)&native_math_cos_entry,
(char*)"native_math_cos_entry"},
{(unsigned char*)&native_math_floor_entry,
(char*)"native_math_floor_entry"},
{(unsigned char*)&native_math_sin_entry,
(char*)"native_math_sin_entry"},
{(unsigned char*)&native_math_sqrt_entry,
(char*)"native_math_sqrt_entry"},
{(unsigned char*)&native_math_tan_entry,
(char*)"native_math_tan_entry"},
{(unsigned char*)&native_string_init_entry,
(char*)"native_string_init_entry"},
{(unsigned char*)&native_string_charAt_entry,
(char*)"native_string_charAt_entry"},
{(unsigned char*)&native_string_endsWith_entry,
(char*)"native_string_endsWith_entry"},
{(unsigned char*)&native_string_equals_entry,
(char*)"native_string_equals_entry"},
{(unsigned char*)&native_string_indexof0_entry,
(char*)"native_string_indexof0_entry"},
{(unsigned char*)&native_string_indexof_entry,
(char*)"native_string_indexof_entry"},
{(unsigned char*)&native_string_indexof0_string_entry,
(char*)"native_string_indexof0_string_entry"},
{(unsigned char*)&native_string_indexof_string_entry,
(char*)"native_string_indexof_string_entry"},
{(unsigned char*)&native_string_startsWith0_entry,
(char*)"native_string_startsWith0_entry"},
{(unsigned char*)&native_string_startsWith_entry,
(char*)"native_string_startsWith_entry"},
{(unsigned char*)&native_string_substringI_entry,
(char*)"native_string_substringI_entry"},
{(unsigned char*)&native_string_substringII_entry,
(char*)"native_string_substringII_entry"},
{(unsigned char*)&native_integer_toString_entry,
(char*)"native_integer_toString_entry"},
{(unsigned char*)&native_stringbuffer_append_entry,
(char*)"native_stringbuffer_append_entry"},
{(unsigned char*)&native_system_arraycopy_entry,
(char*)"native_system_arraycopy_entry"},
{(unsigned char*)&native_vector_addElement_entry,
(char*)"native_vector_addElement_entry"},
{(unsigned char*)&native_vector_elementAt_entry,
(char*)"native_vector_elementAt_entry"},
{(unsigned char*)0, (char*)0}};

#endif
