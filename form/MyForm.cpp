#include "MyForm.h"
#include <WinSock2.h>
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(cli::array<String^>^ arg) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    tmp4::MyForm form;
    Application::Run(% form);
}