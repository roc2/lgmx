#include <cli_iface.h>
#include <iostream>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <debug.h>
#include <exception.h>
#include <view_manager.h>


/**
 * Constructor.
 */

cli_iface::cli_iface(view_manager *parent) : QWidget(parent)
{
	cli_ = new lgmx::cli(parent);
	
	cmd_history_ = new std::list<QString>();
	curr_cmd_ = cmd_history_->begin();
	
	result_ = new QLabel("teste", this);
	
	result_->hide();
	input_ = new QLineEdit(this);
	
	input_->installEventFilter(this);
	
	layout_ = new QHBoxLayout(this);
	layout_->addWidget(input_);
	layout_->addWidget(result_);
	result_->setMaximumHeight (20);
	layout_->setContentsMargins(0, 0, 0, 0);
	layout_->setSpacing(0);
	
	
	this->setContentsMargins (0, 0, 0, 0);
	setLayout(layout_);
	this->setMaximumHeight (22);
}

/**
 * Destructor.
 */

cli_iface::~cli_iface()
{
	delete cmd_history_;
}

/**
 * CLI interface event handler. Handles special keys such as 
 * Enter, to issue a command, and Up and Down keys to repeat 
 * commands.
 */

bool cli_iface::eventFilter(QObject *object, QEvent *event)
{
	if (object == input_ && event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);

		QString cmd(input_->text());
		QString result;
		cmd::stat status;

		switch (ke->key()) {
		case Qt::Key_Return:
		case Qt::Key_Enter:
			
			if (cmd.isEmpty())
				return true;
			
			std::cout << "command issued: " << cmd.toStdString() << std::endl;
			
			// handle command
			status = cli_->execute(cmd, result);

			if (status == cmd::OK) {
				result_->hide();
			} else if (status == cmd::OK_RES) {
				result_->setText("   " + result + "   " );
				result_->setStyleSheet("background-color: rgb(0, 0, 230);");
				result_->show();
			} else {
				result_->setText("   " + result + "   ");
				result_->setStyleSheet("background-color: rgb(200, 0, 0);");
				result_->show();
			}
			
			// add to history
			cmd_history_->push_back(cmd);
			curr_cmd_ = cmd_history_->end();
			input_->clear();
			return true;
			
		case Qt::Key_Up:
			if (curr_cmd_ != cmd_history_->begin()) {
				curr_cmd_--;
				input_->setText(*curr_cmd_);
			}
			return true;
		
		case Qt::Key_Down:
			if (curr_cmd_ == cmd_history_->end() || ++curr_cmd_ == cmd_history_->end())
				input_->clear();
			else
				input_->setText(*curr_cmd_);

			return true;
		
		case Qt::Key_Escape:
			this->hide();
			return true;
		
		default:
			break;
		}
	}
	return false;
}


