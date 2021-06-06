#pragma once
#include "Client.h"
#include <string>
#include <ctime>
#include <msclr/marshal_cppstd.h>

namespace tmp4 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: 
		System::Windows::Forms::TextBox^ ip;
		System::Windows::Forms::Label^ label1;
		System::Windows::Forms::Button^ sendToServer;
		System::Windows::Forms::Button^ connect;
		System::Windows::Forms::Button^ disconnect;
		System::Windows::Forms::TextBox^ console;
		System::Windows::Forms::TextBox^ filePath;
		System::Windows::Forms::Label^ label2;
		Client* client;
	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ip = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->sendToServer = (gcnew System::Windows::Forms::Button());
			this->connect = (gcnew System::Windows::Forms::Button());
			this->disconnect = (gcnew System::Windows::Forms::Button());
			this->console = (gcnew System::Windows::Forms::TextBox());
			this->filePath = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// ip
			// 
			this->ip->Location = System::Drawing::Point(68, 12);
			this->ip->Name = L"ip";
			this->ip->Size = System::Drawing::Size(117, 20);
			this->ip->TabIndex = 0;
			this->ip->Text = L"127.0.0.1";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(50, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"IP-адрес";
			// 
			// sendToServer
			// 
			this->sendToServer->Location = System::Drawing::Point(12, 118);
			this->sendToServer->Name = L"sendToServer";
			this->sendToServer->Size = System::Drawing::Size(173, 23);
			this->sendToServer->TabIndex = 2;
			this->sendToServer->Text = L"Передать серверу";
			this->sendToServer->UseVisualStyleBackColor = true;
			this->sendToServer->Click += gcnew System::EventHandler(this, &MyForm::sendToServer_Click);
			// 
			// connect
			// 
			this->connect->Location = System::Drawing::Point(12, 38);
			this->connect->Name = L"connect";
			this->connect->Size = System::Drawing::Size(81, 23);
			this->connect->TabIndex = 3;
			this->connect->Text = L"Соединиться";
			this->connect->UseVisualStyleBackColor = true;
			this->connect->Click += gcnew System::EventHandler(this, &MyForm::connect_Click);
			// 
			// disconnect
			// 
			this->disconnect->Enabled = false;
			this->disconnect->Location = System::Drawing::Point(99, 38);
			this->disconnect->Name = L"disconnect";
			this->disconnect->Size = System::Drawing::Size(86, 23);
			this->disconnect->TabIndex = 4;
			this->disconnect->Text = L"Отключиться";
			this->disconnect->UseVisualStyleBackColor = true;
			this->disconnect->Click += gcnew System::EventHandler(this, &MyForm::disconnect_Click);
			// 
			// console
			// 
			this->console->Location = System::Drawing::Point(191, 12);
			this->console->Multiline = true;
			this->console->Name = L"console";
			this->console->ReadOnly = true;
			this->console->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->console->Size = System::Drawing::Size(345, 336);
			this->console->TabIndex = 7;
			// 
			// filePath
			// 
			this->filePath->Location = System::Drawing::Point(12, 92);
			this->filePath->Name = L"filePath";
			this->filePath->Size = System::Drawing::Size(173, 20);
			this->filePath->TabIndex = 8;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 76);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(132, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Путь до файла/каталога";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(548, 357);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->filePath);
			this->Controls->Add(this->console);
			this->Controls->Add(this->disconnect);
			this->Controls->Add(this->connect);
			this->Controls->Add(this->sendToServer);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ip);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Client";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
		System::Void connect_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				// Создаем клиенты
				string ip = msclr::interop::marshal_as<std::string>(this->ip->Text);
				client = new Client(ip.c_str());
				// Переключаем кнопки в интерфейсе
				this->disconnect->Enabled = true;
				this->connect->Enabled = false;

				// Формируем строку для вывода в консоль клиента
				string result;
				time_t now = time(0);
				result += "Клиент получил " + (string)ctime(&now) + "\r\n";
				result += client->getAnswer();
				this->console->AppendText(gcnew String(result.c_str()));
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
		
		System::Void disconnect_Click(System::Object^ sender, System::EventArgs^ e) {
			// Удаляем клиент
			delete client;
			// Меняем активность кнопок
			this->disconnect->Enabled = false;
			this->connect->Enabled = true;
		}

		System::Void sendToServer_Click(System::Object^ sender, System::EventArgs^ e) {
			try
			{
				// Формируем строку для вывода в консоль клиента
				string path = msclr::interop::marshal_as<std::string>(this->filePath->Text), result;
				time_t now = time(0);
				result += "Клиент получил " + (string)ctime(&now) + "\r\n";
				result += client->sendMessage(path);
				this->console->AppendText(gcnew String(result.c_str()));
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
	};
}
