/**
 *
 **/


#ifndef DESKTOP_DESKTOPCONNECTOR_H
#define DESKTOP_DESKTOPCONNECTOR_H

#include <QObject>
#include "common/IConnector.h"
class MainWindow;

class DesktopConnector : public QObject, public IConnector
{
    Q_OBJECT
public:

    /// constructor
    explicit DesktopConnector();

    // implementation of IConnector interface
    virtual bool initialize() Q_DECL_OVERRIDE;
    virtual void setState(const QString & path, const QString & newValue) Q_DECL_OVERRIDE;
    virtual QString getState(const QString &path) Q_DECL_OVERRIDE;
    virtual CallbackID addCommandCallback( const QString & cmd, const CommandCallback & cb) Q_DECL_OVERRIDE;
    virtual CallbackID addStateCallback(CSR path, const StateChangedCallback &cb) Q_DECL_OVERRIDE;
    virtual void registerView(IView * view) Q_DECL_OVERRIDE;
    virtual void refreshView(IView *view) Q_DECL_OVERRIDE;

public slots:
    /// javascript calls this to set a state
    void jsSetStateSlot( const QString & key, const QString & value);
    /// javascript calls this to send a command
    void jsSendCommandSlot( const QString & cmd, const QString & parameter);
signals:
    /// we emit this signal when state is changed (either by c++ or by javascript)
    /// we listen to this signal, and so does javascript
    /// our listener then calls callbacks registered for this value
    /// javascript listener caches the new value and also calls registered callbacks
    void stateChangedSignal( const QString & key, const QString & value);
    /// we emit this signal when command results are ready
    /// javascript listens to it
    void jsCommandResultsSignal( const QString & results);

protected slots:
    /// this is the callback for stateChangedSignal
    void stateChangedSlot( const QString & key, const QString & value);

public:

    typedef std::vector<CommandCallback> CommandCallbackList;
    std::map<QString,  CommandCallbackList> m_commandCallbackMap;
    typedef std::vector<StateChangedCallback> StateCBList;
    std::map<QString, StateCBList> m_stateCallbackList;

    CallbackID m_callbackNextId;

    std::map< QString, QString > m_state;

};


#endif // DESKTOP_DESKTOPCONNECTOR_H
