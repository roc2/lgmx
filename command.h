#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QStringList>

class view_manager;

class command
{
public:

	command(QString& name, view_manager* manager);
	
	virtual ~command();

	virtual void param_parser() = 0;

	virtual int execute(QStringList &params, QString &result) = 0;

public:
	QString& get_name() {return name_;}

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

	void param_parser();

	int execute(QStringList &params, QString &result);

};

#endif
