#
# Evironment variables setup
#

isEmpty(APP_SOURCE_TREE):       APP_SOURCE_TREE       = $$PWD
isEmpty(APP_INSTALL_LIBS):      APP_INSTALL_LIBS      = $$APP_SOURCE_TREE/lib
isEmpty(APP_INSTALL_ROOT):      APP_INSTALL_ROOT      = $$APP_SOURCE_TREE/install
isEmpty(APP_INSTALL_BINS):      APP_INSTALL_BINS      = $$APP_INSTALL_ROOT/sys/bin
isEmpty(APP_INSTALL_IMPORTS):   APP_INSTALL_IMPORTS   = $$APP_INSTALL_ROOT/resource/demo/imports
isEmpty(APP_INSTALL_RESOURCES): APP_INSTALL_RESOURCES = $$APP_INSTALL_ROOT/resource/apps/demo

symbian {
	# Symbian-specific paths from
	# [QtSDK]\Symbian\SDKs\[SymbianSDK]\mkspecs\features\symbian\data_caging_paths.prf
	load(data_caging_paths)

	# project-specific
	isEmpty(APP_IMPORTS_BASE_DIR):   APP_IMPORTS_BASE_DIR   = $$RESOURCE_FILES_DIR/demo/imports
	isEmpty(APP_RESOURCES_BASE_DIR): APP_RESOURCES_BASE_DIR = $$APP_RESOURCE_DIR/demo
	
	isEmpty(APP_CAPABILITY): APP_CAPABILITY = NetworkServices ReadUserData UserEnvironment
}
