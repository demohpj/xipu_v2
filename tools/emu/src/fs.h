/*
 * Author: Pawel Jablonski
 * E-mail: pj@xirx.net
 * WWW: xirx.net
 * GIT: git.xirx.net
 *
 * License: You can use this code however you like
 * but leave information about the original author.
 * Code is free for non-commercial and commercial use.
 */

#ifndef FS_H
#define FS_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QFile>
#include <QDir>
#include <QFileInfoList>
#include <QRegularExpression>

//! This class contains file system functions
class FS : public QObject
{
	Q_OBJECT

	public:
		static const int NAME_MAX_LENGTH = 12; //!< Maximum name length of the file

		static const int DIR_ENTRY_SIZE = 16; //!< Size of the header with information about file
		static const int DIR_SIZE_FIELD_SIZE = 2; //!< Size of the size field in the header
		static const int DIR_MAX_QUANTITY = 64; //!< Maximum quantity of files in directory

		static const int FILE_MAX_SIZE = 53248; //!< Maximum size of the file

		static const int MIN_CODE = 0x20; //!< Minimum correctly character code
		static const int MAX_CODE = 0x7e; //!< Maximum correctly character code

		//! This struct contains a size of the file
		struct Size
		{
			unsigned char low; //!< Lower byte of the size
			unsigned char high; //!< Higher byte of the size
		};

		FS(QObject *parent = nullptr);

		void reset();

		void setPath(const QString &path);

		bool open(const QString &path, FS::Size &size);
		bool list(const QString &path, FS::Size &size);

		unsigned char readNextDataByte();

	private:
		QString path; //!< Path to the emulated file system on the local disk
		QByteArray data; //!< Buffer used to loading whole file or a list with description of the files
		int dataPos; //!< Position of the data read pointer
};

#endif
