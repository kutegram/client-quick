<p align="center">
  <a href="https://github.com/kutegram/client-quick">
    <img src="/src/app/gui/view/skin/icons/hicolor/80x80/apps/kutegram.png" width="80" height="80">
  </a>
  <h1>Kutegram</h1>
</p>

## Supported platforms
* Windows XP and higher (maybe even lower?)
* Linux
* Symbian 9.2-9.4
* Symbian^3
* Maemo 5 Fremantle
* MeeGo Harmattan

## Current features
- [x] MTProto 2.0 support
- [x] Code authorization
- [ ] 2FA authorization
- [ ] QR-code login flow
- [x] Dialogs list
- [x] Basic messages history
- [x] Basic messages sending
- [x] Messages entities formatting
- [ ] Media uploading/downloading

## How to build
* Install Qt SDK 1.1.2 or Qt SDK 1.2.1
* If you want to compile for desktop Windows, install [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html) to C:\OpenSSL-Win32
* If you want to build it for Symbian and test on a physical device, install CODA on your phone. You can find the SIS-installer in QtSDK\Symbian\sis\common\CODA
* **_Recursively_** clone the repo: `git clone https://github.com/kutegram/client-quick.git --recursive`
* Open project file - demo.pro
* Go to src\library\ and copy apivalues.default.h as apivalues.h. Uncomment DC_IP, DC_PORT, DC_NUMBER, APP_ID, APP_HASH in apivalues.h and enter the values you got from https://my.telegram.org/apps
* Compile project

