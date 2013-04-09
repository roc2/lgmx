#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

class view_manager;
class QStringList;

namespace cmd {
	enum stat {OK, OK_RES, ERR};
}

class command
{
private:
	command();
	command(const command&);
	command& operator=(const command&);
	
public:
	command(QString& name, view_manager* manager);
	virtual ~command();

	virtual cmd::stat execute(QStringList &params, QString &result) = 0;

public:
	QString get_name();

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

/**
 * Line wrap command.
 */

class line_wrap_cmd : public command
{
public:
	line_wrap_cmd(QString name, view_manager* manager);
	~line_wrap_cmd();

	cmd::stat execute(QStringList &params, QString &result);
};

/**
 * Tags command.
 */

class tags_cmd : public command
{
public:
	tags_cmd(QString name, view_manager* manager);
	~tags_cmd();

	cmd::stat execute(QStringList &params, QString &result);
};

#endif
