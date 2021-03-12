
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "GUIWrapper.h"
#include <iostream>
#include <QFileDialog>
#include <QtGui>
#include <QTextStream>
#include <FrontendWrapper.h>
#include <TNode.h>
#include <vector>
#include <list>
#include "GUINode.h"
#include <QVector>
#include <CFG.h>
#include "CFGHelper.h"
#include "ASTHelper.h"
#include "ASTDrawer.h"
#include "CFGDrawer.h"


using namespace std;
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	scene = new QGraphicsScene(this);

	ui->graphicsView->setScene(scene);

	wrapper = new GUIWrapper();

}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);

	if (ast_drawn_) {
		EndSetASTScene();
	}
	else {
		EndSetCFGScene();
	}
}

void MainWindow::btnLoad_clicked() {
	
	try {

		QString src_name = QFileDialog::getOpenFileName(this,
			tr("Open Source file"), ".", tr("txt Files (*.txt)"));

		QFile file;
		file.setFileName(src_name);
		if (!file.open(QIODevice::ReadOnly))
			return;
		file_contents_ = file.readAll();
		ui->txtCodeEditor->document()->setPlainText(file_contents_);	
		wrapper->parse(src_name.toStdString());
		on_astButton_clicked();

	}
	catch (exception& e) {
		SetErrorMessage(e.what());
	}
	catch (char* except) {
		SetErrorMessage(except);
	}
}

void MainWindow::btnRun_clicked() {

	if (file_contents_ == NULL) {
		SetErrorMessage("No loaded file found.");
		return;
	}

	try {
		std::list<std::string> results;
		wrapper->Evaluate(ui->txtQuery->toPlainText().toStdString(),results);

		QString result_str;

		int i = 0;

		PrintResults(results);

		results_ = results;

		if (ast_drawn_) {
			DrawAST();
		}
		else {
			DrawCFG();
		}
	}
	catch (exception& e) {
		SetErrorMessage(e.what());
	}
	catch (char* except) {
		SetErrorMessage(except);
	}


}

void MainWindow::on_astButton_clicked() {

	if (file_contents_ == NULL) {
		SetErrorMessage("No loaded file found.");
		return;
	}

	try {
		ast_drawn_ = true;

		ast_helper_ = new ASTHelper(file_contents_.toUtf8().constData());

		DrawAST();
	}
	catch (exception& e) {
		SetErrorMessage(e.what());
	}
	catch (char* except) {
		SetErrorMessage(except);
	}


}


void MainWindow::on_cfgButton_clicked() {
	
	if (file_contents_ == NULL) {
		SetErrorMessage("No loaded file found.");
		return;
	}

	try {
		ast_drawn_ = false;

		cfg_helper_ = new CFGHelper(file_contents_.toUtf8().constData());

		DrawCFG();
	}
	catch (exception& e) {
		SetErrorMessage(e.what());
	}
	catch (char* except) {
		SetErrorMessage(except);
	}


}

void MainWindow::DrawCFG() {


	BeginSetScene();

	cfg_drawer_ = new CFGDrawer(scene, cfg_helper_->GetCFGNodeMap());
	
	cfg_drawer_->DrawCFG();

	if (results_.size() > 0) {
		cfg_drawer_->ColorResults(results_);
	}

	EndSetCFGScene();

}

void MainWindow::DrawAST() {

	BeginSetScene();

	ast_drawer_ = new ASTDrawer(scene, ast_helper_->GetGUINodeTree());

	ast_drawer_->DrawAST();

	if (results_.size() > 0) {
		ast_drawer_->ColorResults(results_);
	}

	EndSetASTScene();

}

void MainWindow::BeginSetScene() {
	scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);
	ui->verticalSlider->setValue(1);
}

void MainWindow::EndSetASTScene() {
	//ui->graphicsView->fitInView(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
	ui->graphicsView->fitInView(ui->graphicsView->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::EndSetCFGScene() {
	ui->graphicsView->fitInView(ui->graphicsView->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::PrintResults(list<string> results) {

	QString result_str;
	int i = 0;

	for (auto const& result : results) {

		result_str += QString(result.c_str());
		if (i < results.size() - 1) {
			result_str += ", ";
		}
		i += 1;
	}

	ui->txtResult->document()->setPlainText(result_str);
}

void MainWindow::on_verticalSlider_valueChanged(int value) {

	if (value < prev_value) {
		ui->graphicsView->scale(0.85, 0.85);
	}
	else {
		ui->graphicsView->scale(1.0 / 0.85, 1.0 / 0.85);
	}

	prev_value = value;

}

void MainWindow::SetErrorMessage() {
	ui->txtResult->setHtml(QString::fromStdString("<span style=\"color:red\">An error occurred. Please contact CS3219 T13 for clarification<span>"));
}

void MainWindow::SetErrorMessage(string msg) {
	ui->txtResult->setHtml(QString::fromStdString("<span style=\"color:red\">" + msg + "<span>"));
}