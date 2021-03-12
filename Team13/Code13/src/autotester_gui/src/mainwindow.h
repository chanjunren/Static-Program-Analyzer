#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "GUIWrapper.h"
#include <TNode.h>
#include <QGraphicsItem>
#include "GUINode.h"
#include "CFG.h"
#include "ASTHelper.h"
#include "CFGHelper.h"
#include "ASTDrawer.h"
#include "CFGDrawer.h"

typedef vector<GUINode*> NODE_LIST;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


void resizeEvent(QResizeEvent* event);


public slots:
	void btnLoad_clicked();
	void btnRun_clicked();
	void on_astButton_clicked();
	void on_cfgButton_clicked();
	void on_verticalSlider_valueChanged(int value);

private:
	Ui::MainWindow *ui;
	GUIWrapper *wrapper;
	QGraphicsScene* scene;
	QString file_contents_;
	ASTHelper* ast_helper_;
	CFGHelper* cfg_helper_;
	ASTDrawer* ast_drawer_;
	CFGDrawer* cfg_drawer_;
	list<string> results_;
	bool ast_drawn_;
	int prev_value = 1;

	QBrush brush_primary_;
	QPen outline_pen_;

	void DrawAST();
	void DrawCFG();
	void PrintResults(list<string> results);
	void BeginSetScene();
	void EndSetASTScene();
	void EndSetCFGScene();
	void SetErrorMessage();
	void SetErrorMessage(string msg);


};

#endif // MAINWINDOW_H
