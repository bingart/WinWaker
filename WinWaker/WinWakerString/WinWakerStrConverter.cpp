
#include "WinWakerStrConverter.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <Windows.h>
#include "argument.h"
#include "str.h"
#include "file.h"

extern std::string timeString;

BOOL WinWakerStrConverter::Convert(const char* txtFilePath, const char* cppFilePath, bool isLib)
{
	GetTimeString();

	BOOL rc = FALSE;
	FILE* in = NULL;
	FILE* out = NULL;

	std::string szGetStrFunctionName;
	std::string szStrBufferName;
	std::string szCriticalSectionName;
	std::string szInitCriticalSectionName;

	if (isLib)
	{
		szGetStrFunctionName = "GetLibStrById";
		szStrBufferName = "szLibStrBuffers";
		szCriticalSectionName = "csLibCriticalSection";
		szInitCriticalSectionName = "bLibInitCriticalSection";
	}
	else
	{
		szGetStrFunctionName = "GetStrById";
		szStrBufferName = "szStrBuffers";
		szCriticalSectionName = "csCriticalSection";
		szInitCriticalSectionName = "bInitCriticalSection";
	}

	do
	{
		// open txt file
		in = fopen(txtFilePath, "r");
		if (in == NULL)
		{
			break;
		}

		out = fopen(cppFilePath, "w");
		if (out == NULL)
		{
			break;
		}
		else
		{
			// write cpp file
			fprintf(out, "\n");
			fprintf(out, "////////////////////////////////////////////////\n");
			fprintf(out, "// Created by WinWakerString at %s\n", timeString.c_str());
			fprintf(out, "////////////////////////////////////////////////\n");
			fprintf(out, "\n");
			fprintf(out, "#include \"libstr.h\"\n");
			fprintf(out, "#include \"str.h\"\n");
			fprintf(out, "#include <string>\n");
			fprintf(out, "\n");
			fprintf(out, "char* %s[1024] = {0};\n", szStrBufferName.c_str());
			fprintf(out, "CRITICAL_SECTION %s;\n", szCriticalSectionName.c_str());
			fprintf(out, "BOOL %s = FALSE;\n", szInitCriticalSectionName.c_str());
			fprintf(out, "\n");
			fprintf(out, "char* %s(int id)\n", szGetStrFunctionName.c_str());
			fprintf(out, "{\n");

			fprintf(out, "\tif (!%s)\n", szInitCriticalSectionName.c_str());
			fprintf(out, "\t{\n");
			fprintf(out, "\t\tInitializeCriticalSection(&%s);\n", szCriticalSectionName.c_str());
			fprintf(out, "\t\tfor (int i = 0; i < 1024; i++)\n");
			fprintf(out, "\t\t{\n");
			fprintf(out, "\t\t\t%s[i] = NULL;\n", szStrBufferName.c_str());
			fprintf(out, "\t\t}\n");
			fprintf(out, "\t\t%s = TRUE;\n", szInitCriticalSectionName.c_str());
			fprintf(out, "\t}\n");
			fprintf(out, "\n");

			fprintf(out, "\tEnterCriticalSection(&%s);\n", szCriticalSectionName.c_str());
			fprintf(out, "\n");

			fprintf(out, "\tif (true)\n");
			fprintf(out, "\t{\n");
			fprintf(out, "\t\tif (id < 0 || id >= 1024)\n");
			fprintf(out, "\t\t{\n");
			fprintf(out, "\t\t\tLeaveCriticalSection(&%s);\n", szCriticalSectionName.c_str());
			fprintf(out, "\t\t\treturn \"\";\n");
			fprintf(out, "\t\t}\n");
			fprintf(out, "\n");

			fprintf(out, "\t\tif (%s[id] == NULL)\n", szStrBufferName.c_str());
			fprintf(out, "\t\t{\n");
			fprintf(out, "\t\t\t%s[id] = new char[MAX_PATH];\n", szStrBufferName.c_str());
			fprintf(out, "\t\t\tmemset(%s[id], 0, MAX_PATH);\n", szStrBufferName.c_str());
			fprintf(out, "\t\t\tif (%s[id] == NULL)\n", szStrBufferName.c_str());
			fprintf(out, "\t\t\t{\n");
			fprintf(out, "\t\t\t\tLeaveCriticalSection(&%s);\n", szCriticalSectionName.c_str());
			fprintf(out, "\t\t\t\treturn \"\";\n");
			fprintf(out, "\t\t\t}\n");
			fprintf(out, "\t\t}\n");
			fprintf(out, "\t\telse\n");
			fprintf(out, "\t\t{\n");
			fprintf(out, "\t\t\tLeaveCriticalSection(&%s);\n", szCriticalSectionName.c_str());
			fprintf(out, "\t\t\treturn %s[id];\n", szStrBufferName.c_str());
			fprintf(out, "\t\t}\n");
			fprintf(out, "\t}\n");
			fprintf(out, "\n");

			fprintf(out, "\tstd::string encryptStr;\n");
			fprintf(out, "\n");

			fprintf(out, "\tswitch(id)\n");
			fprintf(out, "\t{\n");
		}

		char buffer[MAX_PATH];
		std::vector<std::string> arguments;
		char rawStr[MAX_PATH] = {0}; 
		char encryptStr[MAX_PATH] = {0};

		while (!feof(in))
		{
			memset(buffer, 0, sizeof buffer);
			memset(rawStr, 0, sizeof rawStr);
			memset(encryptStr, 0, sizeof encryptStr);
			arguments.clear();

			if (fgets(buffer, sizeof buffer, in) != NULL)
			{
				// remove the linefeed
				size_t len = strlen(buffer);
				if (buffer[len - 1] == '\n')
				{
					buffer[len - 1] = '\0';
				}

				// remove the CR
				len = strlen(buffer);
				if (buffer[len - 1] == '\r')
				{
					buffer[len - 1] = '\0';
				}

				std::string line = buffer;

				// replace "\\n" with "\n"
				size_t pos;   
				if((pos = line.find("\\n")) != std::string::npos)
				{
					line.replace(pos, 2, "\n"); 
				}

				BOOL rc2 = GetArguments(line, '@', arguments);
				if (rc2 && arguments.size() == 2)
				{
					// prepare id and raw str
					int id = atoi(arguments[0].c_str());
					strncpy(rawStr, arguments[1].c_str(), sizeof rawStr);

					// encrypt raw str
					EncryptString(rawStr, strnlen(rawStr, sizeof rawStr), encryptStr, sizeof encryptStr);

					// write cpp file
					fprintf(out, "\tcase %d:\n", id);
					fprintf(out, "\t\t// raw str -> \"%s\"\n", buffer);
					fprintf(out, "\t\tencryptStr = \"%s\";\n", encryptStr);
					fprintf(out, "\t\tbreak;\n");
				}
			}
		}

		// write cpp file
		fprintf(out, "\tdefault:\n");
		fprintf(out, "\t\tbreak;\n");
		fprintf(out, "\t}\n");
		fprintf(out, "\n");

		fprintf(out, "\tDecryptString(encryptStr.c_str(), encryptStr.size(), %s[id], MAX_PATH);\n", szStrBufferName.c_str());
		fprintf(out, "\n");

		fprintf(out, "\tLeaveCriticalSection(&%s);\n", szCriticalSectionName.c_str());
		fprintf(out, "\n");

		fprintf(out, "\treturn %s[id];\n", szStrBufferName.c_str());
		fprintf(out, "}\n");

		rc = TRUE;

	} while(false);

	if (in != NULL) fclose(in);
	if (out != NULL) fclose(out);

	return rc;
}