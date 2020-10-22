#ifndef STREAMSINK_H
#define STREAMSINK_H

#pragma warning(push, 0)
#include <QtCore/QObject>
#include <QtCore/QString>
#include <streambuf>
#include <iostream>
#include <string>
#pragma warning(pop)

class StreamSink : public QObject, public std::basic_streambuf<char>
{
    Q_OBJECT
public:
    StreamSink(std::ostream& oStream)
        : _oStream(oStream)
        , _pOldStreamBuf(oStream.rdbuf())
    {
        oStream.rdbuf(this);
    }
    ~StreamSink()
    {
        if(!_sString.empty())
        {
            emit toLogs(QString::fromStdString(_sString));
        }

        _oStream.rdbuf(_pOldStreamBuf);
    }

signals:
    void toLogs(const QString& sMessage);

protected:
    virtual int_type overflow(int_type c) override
    {
        _sString += c;
        if (c == '\n')
        {
            emit toLogs(QString::fromStdString(_sString));
            _sString.clear();
        }
        return c;
    }

    virtual std::streamsize xsputn(const char* const p, const std::streamsize n) override
    {
        _sString.append(p, p + n);

        long pos = 0;
        while (pos != static_cast<long>(std::string::npos))
        {
            pos = static_cast<long>(_sString.find('\n'));
            if (pos != static_cast<long>(std::string::npos))
            {
                std::string tmp(_sString);
                emit toLogs(QString::fromStdString(tmp));
                _sString.clear();
            }
        }

        return n;
    }

private:
    std::ostream& _oStream;
    std::streambuf* const _pOldStreamBuf = nullptr;
    std::string _sString;
};

#endif // STREAMSINK_H
