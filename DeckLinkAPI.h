#ifndef DECKLINKAPI_H
#define DECKLINKAPI_H

#include <QMetaType>
#include <QString>

#if defined(Q_OS_WIN)
#include "sdk/Win/DeckLinkAPI_h.h"
typedef IID CFUUIDBytes;
#elif defined(Q_OS_MACX)
#include "sdk/Mac/include/DeckLinkAPI.h"

typedef bool BOOL;

class DLString {
private:
	CFStringRef str = nullptr;
public:
	~DLString();
	void clear();
	CFStringRef *operator & ()
	{
		return &str;
	}
	operator QString () const
	{
		if (!str) return QString();
		CFIndex length = CFStringGetLength(str);
		if (length == 0) return QString();
		QString string(length, Qt::Uninitialized);
		CFStringGetCharacters(str, CFRangeMake(0, length), reinterpret_cast<UniChar *>(const_cast<QChar *>(string.unicode())));
		return string;
	}
	operator std::string () const
	{
		return operator QString ().toStdString();
	}
	bool empty() const
	{
		return !(str && CFStringGetLength(str) > 0);
	}
};

#else
#include "sdk/Linux/include/DeckLinkAPI.h"
typedef bool BOOL;
//typedef char *BSTR;

#endif

#ifdef Q_OS_MAC
#else
class DLString {
private:
#ifdef Q_OS_WIN
	BSTR str = nullptr;
#endif
#ifdef Q_OS_LINUX
	char const *str = nullptr;
#endif
public:
	~DLString();
	void clear();
#ifdef Q_OS_WIN
	BSTR *operator & ()
	{
		return &str;
	}
	operator QString ()
	{
		return str ? QString::fromUtf16((ushort const *)str) : QString();
	}
	operator std::string ()
	{
		return operator QString ().toStdString();
	}
	bool empty() const
	{
		return !(str && *str);
	}
#endif
#ifdef Q_OS_LINUX
	char const **operator & ()
	{
		return &str;
	}
	operator QString () const
	{
		return str ? QString::fromUtf8(str) : QString();
	}
	operator std::string () const
	{
		return str ? std::string(str) : std::string();
	}
	bool empty() const
	{
		return !(str && *str);
	}
#endif
};
#endif

#endif // DECKLINKAPI_H
