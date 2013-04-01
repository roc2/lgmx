#ifndef TAGS_H
#define TAGS_H

#include <QObject>
#include <readtags.h>
#include <QString>
#include <list>
#include <vector>

class view_manager;
class QStringListModel;
class QListView;

/**
 * This class relies on exuberant ctags 5.8 API for tags search.
 */

class tag : public QObject
{
	Q_OBJECT

private:
	tag(const tag&);
	tag& operator=(const tag&);

public:
	tag(view_manager *manager);
	~tag();

	bool add_tags_file(const QString &file_name, QString &res);
	bool remove_tags_file(const QString &file_name, QString &err);
	void clear_tags();
	
	bool find_tag(const QString &name);
	void show_tags_list(QStringList &matches);

private:
	bool eventFilter(QObject *obj, QEvent *event);

signals:
	void jump_to(const QString &, int);

private:
	view_manager *manager_;
	std::list<QString> tag_files_;
	
	QStringListModel *model_;
	QListView *view_;
	std::vector<std::pair<QString, int> > tag_matches_;
};

#endif
