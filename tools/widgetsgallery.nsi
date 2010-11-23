; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Widgets Gallery"
!define PRODUCT_VERSION "0.19.3"
!define PRODUCT_PUBLISHER "Nokia"
!define PRODUCT_WEB_SITE "http://www.nokia.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\widgetsgallery.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
;!insertmacro MUI_PAGE_LICENSE "c:\path\to\licence\YourSoftwareLicence.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\widgetsgallery.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"

; this path seems to be relative from the widgetsgallery.nsi location
OutFile "..\..\WidgetsGallerySetup.exe"

InstallDir "$PROGRAMFILES\WidgetsGallery"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show


Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer

  ; dui files
  File /r "..\..\inst\*.*"

  CreateDirectory "$SMPROGRAMS\Widgets Gallery"
  CreateShortCut "$SMPROGRAMS\Widgets Gallery\Widgets Gallery.lnk" "$INSTDIR\bin\widgetsgallery.exe"
  CreateShortCut "$DESKTOP\Widgets Gallery.lnk" "$INSTDIR\bin\widgetsgallery.exe"
SectionEnd


;Section -Post
;  WriteUninstaller "$INSTDIR\uninst.exe"
;  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\widgetsgallery.exe"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\widgetsgallery.exe"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
;  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
;SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

;Section Uninstall
;  Delete "$INSTDIR\${PRODUCT_NAME}.url"
;  Delete "$INSTDIR\uninst.exe"
;  Delete "$INSTDIR\Example.file"
;  Delete "$INSTDIR\AppMainExe.exe"

;  Delete "$SMPROGRAMS\Widgets Gallery\Uninstall.lnk"
;  Delete "$SMPROGRAMS\Widgets Gallery\Website.lnk"
;  Delete "$DESKTOP\Widgets Gallery.lnk"
;  Delete "$SMPROGRAMS\Widgets Gallery\Widgets Gallery.lnk"

;  RMDir "$SMPROGRAMS\Widgets Gallery"
;  RMDir "$INSTDIR"

;  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
;  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
;  SetAutoClose true
;SectionEnd
