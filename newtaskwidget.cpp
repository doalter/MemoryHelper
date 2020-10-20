#include "newtaskwidget.h"
#include <QMessageBox>

NewTaskWidget::NewTaskWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pbSaveMemory, SIGNAL(clicked()), this, SLOT(onSaveMemory()));
}

NewTaskWidget::~NewTaskWidget()
{
}

void NewTaskWidget::onSaveMemory()
{
	QString newSentence = ui.textNewMemory->toPlainText();
	if (newSentence.isEmpty())
	{
		QMessageBox::information(this, "INFO", "NO date to save!");
		ui.textKeywords->setPlainText("");  //clear pain text.
		return;
	}
	QString keywords = ui.textKeywords->toPlainText();
	RawMemory rawMem({ newSentence, keywords });
	emit saveMemorySignal(rawMem);
	QMessageBox msgBox(QMessageBox::Question, tr("张小宝"), "New memory saved! Add more task or Exit?", 
					   QMessageBox::Yes | QMessageBox::No);
	msgBox.button(QMessageBox::Yes)->setText("Add");
	msgBox.button(QMessageBox::No)->setText("Exit");
	auto result = msgBox.exec();
	if (result == QMessageBox::Yes) 
	{
		ui.textNewMemory->setPlainText("");  //clear pain text.
		ui.textKeywords->setPlainText("");  //clear pain text.
	}
	else 
	{
		this->close();
	}


}