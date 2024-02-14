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

#include "fs.h"

/**
 * Constructor for the file system class
 *
 * @param parent Parent object
 */
FS::FS(QObject *parent) : QObject(parent)
{
	this->path = "";

	this->reset();
}

//! Reset the file system buffers and path
void FS::reset()
{
	this->data.clear();
	this->dataPos = 0;
}

/**
 * Set path to the emulated file system on the local disk
 *
 * @param path Path to emulated file system
 */
void FS::setPath(const QString &path)
{
	this->path = path;
}

/**
 * Open and load to buffer a file from the emulated file system
 *
 * @param path Path to a file
 * @param size Struct where size of the file will be written
 *
 * @return Status of the open operation
 */
bool FS::open(const QString &path, FS::Size &size)
{
	size.low = 0;
	size.high = 0;

	if(!path.contains(QRegularExpression("^(([0-9a-zA-Z_-]{1,8})/)?([0-9a-zA-Z_-]{1,8})(([\\\\.])([0-9a-zA-Z_-]{1,3}))?$")))
	{
		return(false);
	}

	QString filePath = QString(this->path).append("/").append(path);
	QFile file(filePath);

	if(!file.open(QIODevice::ReadOnly))
	{
		return(false);
	}

	if(file.size() > FILE_MAX_SIZE)
	{
		file.close();

		return(false);
	}

	this->data = file.readAll();

	file.close();

	this->dataPos = 0;

	size.low = static_cast<unsigned char>(this->data.size() % 256);
	size.high = static_cast<unsigned char>((this->data.size() / 256) & 0xff);

	return(true);
}

/**
 * Create a list of files in the directory from the emulated file system
 *
 * @param path Path to a directory
 * @param size Struct where size of the buffer with description of the files will be written
 *
 * @return Status of the list operation
 */
bool FS::list(const QString &path, FS::Size &size)
{
	size.low = 0;
	size.high = 0;

	if(!path.contains(QRegularExpression("^([0-9a-zA-Z_-]{1,8})$")))
	{
		return(false);
	}

	QString dirPath = QString(this->path).append("/").append(path);
	QDir dir;

	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::Name);

	if(!dir.cd(dirPath))
	{
		return(false);
	}

	QFileInfoList fileInfoList = dir.entryInfoList();

	this->data.clear();

	for(const QFileInfo &fileInfo : fileInfoList)
	{
		QString fileName = fileInfo.fileName();
		qint64 fileSize = fileInfo.size();

		if(fileSize > FILE_MAX_SIZE)
		{
			continue;
		}

		if(!fileName.contains(QRegularExpression(R"(^([0-9a-zA-Z_-]{1,8})(([\\.])([0-9a-zA-Z_-]{1,3}))?$)")))
		{
			continue;
		}

		int j = 0;

		for(; ((j < NAME_MAX_LENGTH) && (j < fileName.length())); j++)
		{
			this->data.append(fileName.at(j).toLatin1());
		}

		for(; j < (DIR_ENTRY_SIZE - DIR_SIZE_FIELD_SIZE); j++)
		{
			this->data.append(static_cast<char>(0));
		}

		QVector<unsigned char> size(2);

		size[0] = static_cast<unsigned char>(fileSize % 256);
		size[1] = static_cast<unsigned char>((fileSize / 256) & 0xff);

		this->data.append(static_cast<char>(size[0]));
		this->data.append(static_cast<char>(size[1]));

		if(this->data.size() >= (DIR_MAX_QUANTITY * DIR_ENTRY_SIZE))
		{
			break;
		}
	}

	this->dataPos = 0;

	size.low = static_cast<unsigned char>(this->data.size() % 256);
	size.high = static_cast<unsigned char>((this->data.size() / 256) & 0xff);

	return(true);
}

/**
 * Get next byte from the buffer
 *
 * @return Next read byte from the buffer
 */
unsigned char FS::readNextDataByte()
{
	unsigned char data = 0;

	if(this->data.length() > this->dataPos)
	{
		data = static_cast<unsigned char>(this->data.at(this->dataPos));

		this->dataPos++;
	}

	return(data);
}
