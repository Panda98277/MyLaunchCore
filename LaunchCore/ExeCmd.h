#pragma once
#include "pch.h"


using namespace std;
namespace UnknownMinecraftLaunchCore {
  namespace Toolkits {
    wstring Str2Wstr(string str) {
      unsigned len = str.size() * 2;// Ԥ���ֽ���
      setlocale(LC_CTYPE, "");     //������ô˺���
      wchar_t* p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
      mbstowcs(p, str.c_str(), len);// ת��
      std::wstring str1(p);
      delete[] p;// �ͷ�������ڴ�
      return str1;
    }
    string ExeCmd(string pszCmd) {
      wstring pszCmd_w = Str2Wstr(pszCmd);
      wcout << "pszCmd_w is " << pszCmd_w << endl;
      // ���������ܵ�,write->read;
      SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
      HANDLE hRead, hWrite;
      if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        cout << "@ CreatePipe failed!" << endl;
        return (" ");
      }
      cout << "@0" << endl;
      // ���������н���������Ϣ(�����ط�ʽ���������λ�������hWrite
      STARTUPINFO si = { sizeof(STARTUPINFO) }; // Pointer to STARTUPINFO structure;
      GetStartupInfo(&si);
      si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
      //si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE; //���ش��ڣ�
      si.hStdError = hWrite;
      si.hStdError = hWrite;
      si.hStdOutput = hWrite; //�ܵ�������˿����������е������
      // ����������
      PROCESS_INFORMATION pi;// Pointer to PROCESS_INFORMATION structure;
      if (!CreateProcess(NULL,
        (LPWSTR)pszCmd_w.c_str(),
        NULL,
        NULL,
        TRUE,
        //FALSE,          // Set handle inheritance to FALSE
        NULL,
        //0,              // No creation flags
        NULL,
        NULL,
        &si,
        &pi)) {
        cout << "@ CreateProcess failed!" << endl;
        return ("Cannot create process");
      }
      CloseHandle(hWrite);//�رչܵ�������˿ڣ�
      // ��ȡ�����з���ֵ
      string strRetTmp;
      char buff[1024] = { 0 };
      DWORD dwRead = 0;
      strRetTmp = buff;
      while (ReadFile(hRead, buff, 1024, &dwRead, NULL))//�ӹܵ�������˻�ȡ������д������ݣ�
      {
        cout << "buff = " << buff << endl;
        strRetTmp += buff;
      }
      CloseHandle(hRead);//�رչܵ�������˿ڣ�
      cout << "strRetTmp:" << strRetTmp << endl;
      return strRetTmp;
    }
  }
}