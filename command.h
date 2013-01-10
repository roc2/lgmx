#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>

class view_manager;

namespace cmd {
	enum stat {OK, OK_RES, ERR};
}

class command
{
public:
	command(QString& name, view_manager* manager);
	virtual ~command();

	virtual cmd::stat execute(QStringList &params, QString &result) = 0;

public:
	QString& get_name();

protected:
	QString name_;
	view_manager *manager_;
	int multiplicity_;
	bool write_;
};


/**
 * Tab width command.
 */

class tab_width_cmd : public command
{
public:
	tab_width_cmd(QString name, view_manager* manager);
	~tab_width_cmd();

	cmd::stat execute(QStringList &params, QString &result);
};

/**
 * Version command.
 */

class version_cmd : public command
{
public:
	version_cmd(QString name, view_manager* manager);
	~version_cmd();

	cmd::stat execute(QStringList &params, QString &result);
};

#endif
