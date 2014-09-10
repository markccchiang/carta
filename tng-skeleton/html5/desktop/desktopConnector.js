/**
 * Desktop connector. See IConnector.js for documentation regarding the API. This is just
 * an implementation.
 */

/* JsHint options */
/*global mExport, mImport, QtConnector, QtConnector.* */
/*jshint eqnull:true */


(function()
{
    "use strict";

    var connector = mExport( "connector", {} );
    var setZeroTimeout = mImport( "setZeroTimeout" );
    var console = mImport( "console" );
    var defer = mImport( "defer" );

    /**
     * Numerical constants representing status of the connection.
     * @type {{}}
     */
    connector.CONNECTION_STATUS = {
        CONNECTED   : 1,
        CONNECTING  : 2,
        FAILED      : 3,
        STALLED     : 4,
        DISCONNECTED: 5,
        UNKNOWN     : 6
    };

    connector.VIEW_CALLBACK_RESON = {
        UPDATED   : 1,
        TX_CHANGED: 2
    };

    // private variables
    var m_connectionStatus = connector.CONNECTION_STATUS.DISCONNECTED;
    var m_connectionCB = null;
    // last callback ID, so we can generate unique ones
    var m_lastCallbackID = 1;
    // we keep following information for every state:
    //  - path (so that individual shared variables don't need to keep their own copies)
    //  - value
    //  - callbacks
    // for each callback we keep
    //    - callback {function}
    //    - id
    // We start with an empty state
    var m_states = {};
    // array of callbacks for commands, these are used to report back results
    var m_commandCallbacks = [];
    // map of views
    var m_views = {};

    // listen for command results callbacks and always invoke the top callback in the list
    // the command results always arrive in the same order they were sent
    QtConnector.jsCommandResultsSignal.connect( function( result) {
        if( m_commandCallbacks.length < 1) {
            console.warn( "Received command results but no callbacks for this!!!");
            console.warn( "The result: ", result);
            return;
        }
        var cb = m_commandCallbacks.shift();
        if( cb == null) {
            // skip this callback
            return;
        }
        if( typeof cb !== "function") {
            console.warn( "Registered callback for command is not a function!");
        } else {
            cb( result);
        }
    });

    // listen for jsViewUpdatedSignal to render the image
    QtConnector.jsViewUpdatedSignal.connect( function( viewName, buffer) {
        var view = m_views[ viewName];
        if( view == null) {
            console.warn( "Ignoring update for unconnected view '" + viewName + "'");
            return;
        }
        buffer.assignToHTMLImageElement( view.m_imgTag);
    });

    // convenience function to create & get or just get a state
    function getOrCreateState( path )
    {
        var st = m_states[path];
        if( st !== undefined ) {
            return st;
        }
        st = { path: path, value: null, callbacks: [] };
        m_states[path] = st;
        return st;
    }

    // the View class
    var View = function( container, viewName )
    {
        // QtWebKit does not support drawing to the canvas (they claim they do, but
        // it coredumps). So we'll use <img> tag instead. That works well enough.
        // TODO: investigate performance using QWebFactoryPlugin vs <img> tag

        // create an image tag inside the container
        this.m_container = container;
        this.m_viewName = viewName;
        this.m_imgTag = document.createElement( "img" );
        this.m_imgTag.setAttribute( "max-width", "100%");
        this.m_imgTag.setAttribute( "max-height", "100%");
//        console.log( "imgTag = ", this.m_imgTag );
        this.m_container.appendChild( this.m_imgTag );

        this.m_imgTag.onmousemove = function( ev) {
            var x = ev.pageX - this.m_imgTag.getBoundingClientRect().left;
            var y = ev.pageY - this.m_imgTag.getBoundingClientRect().top;
            console.log( "jsMouseMoveSlot", this.m_viewName, x, y );
            QtConnector.jsMouseMoveSlot( this.m_viewName, x, y);
        }.bind(this);
    };
    View.prototype.setQuality = function setQuality()
    {
        // desktop does not have quality
    };
    View.prototype.updateSize = function()
    {
//        this.m_imgTag.width = this.m_container.offsetWidth;
//        this.m_imgTag.height = this.m_container.offsetHeight;
        console.log( "about to call jsUpdateViewSlot",  this.m_viewName, this.m_container.offsetWidth, this.m_container.offsetHeight );
        QtConnector.jsUpdateViewSlot( this.m_viewName, this.m_container.offsetWidth, this.m_container.offsetHeight);
    };
    View.prototype.getName = function()
    {
        return this.m_viewName;
    };
    View.prototype.getServerSize = function()
    {
        return { width: 99, height: 101};
    };
    View.prototype.local2server = function( coordinate )
    {
        return coordinate;
    };
    View.prototype.server2local = function( coordinate )
    {
        return coordinate;
    };
    View.prototype.addViewCallback = function( callback )
    {
    };

    connector.registerViewElement = function( divElement, viewName )
    {
        var view = m_views[ viewName];
        if( view !== undefined) {
            throw new Error("Trying to re-register existing view '" + viewName + "'");
        }
        view = new View( divElement, viewName );
        m_views[ viewName] = view;
        return view;
    };

    connector.setInitialUrl = function( /*url*/ )
    {
        // we don't need urls
    };

    connector.getConnectionStatus = function()
    {
        return m_connectionStatus;
    };


    connector.setConnectionCB = function( callback )
    {
        m_connectionCB = callback;
    };

    connector.connect = function()
    {
        if( m_connectionCB == null ) {
            console.warn( "No connection callback specified!!!" );
        }

        if( window.QtPlatform !== undefined || window.QtConnector !== undefined ) {
            m_connectionStatus = connector.CONNECTION_STATUS.CONNECTED;
        }

        if( m_connectionCB != null ) {
            m_connectionCB();
        }

        // listen for changes to the state
        QtConnector.stateChangedSignal.connect( function( key, val )
        {
            console.log( "qt.stateChanged", key, val );
            var st = getOrCreateState( key );
            st.value = val;
            // now go through all callbacks and call them
            st.callbacks.forEach( function( cb )
            {
                cb.callback( st.value );
            } );
        } );
    };

    connector.disconnect = function()
    {
    };

    connector.canShareSession = function()
    {
        return false;
    };

    connector.shareSession = function( /*callback, username, password, timeout*/ )
    {
    };

    connector.unShareSession = function( /*errorCallback*/ )
    {
    };

    function SharedVar( path )
    {
        console.log( "Creating shared variable:", path );

        // make a copy of this to use in private/priviledged functions
        var m_that = this;
        // save a pointer to the list of states
        var m_statePtr = getOrCreateState( path );
        // maintain our own list of callbacks so that we can remove them when
        // destroy() gets called
        var m_myCallbackIDs = [];


        // add a callback for the variable
        this.addNamedCB = function( callback )
        {
            if( typeof callback !== "function" ) {
                throw "callback is not a function!!";
            }
            // generate the ID for this callback
            var cbId = "cb" + (m_lastCallbackID ++);
            // add callback to the list of all callbacks for this state key
            m_statePtr.callbacks.push( { callback: callback, id: cbId } );
            // add this ID to our own list
            m_myCallbackIDs.push( cbId);
            // return the id
            return cbId;
        };

        // add an anonymous callback
        this.addCB = function( callback )
        {
            m_that.addNamedCB( callback );
            return m_that;
        };

        this.set = function( value )
        {
            if( typeof value === "boolean" ) {
                value = value ? "1" : "0";
            }
            else if( typeof value === "string" ) {
                // do nothing, this will be verbatim
                value = value;
            }
            else if( typeof value === "number" ) {
                // convert number
                value = "" + value;
            }
            else {
                console.error( "value has weird type: ", value, m_statePtr.path );
                throw "don't know how to set value";
            }
            QtConnector.jsSetStateSlot( m_statePtr.path, value );

            return m_that;
        };

        this.get = function()
        {
            return m_statePtr.value;
        };

        // this should be called when the variable will no longer be used, so that
        // callbacks associated with this var can be erased
        // TODO: we should only remove callbacks registered via this var
        this.destroy = function()
        {
            m_myCallbackIDs.forEach( function(cbId) {
                m_that.removeCB( cbId);
            });
            m_myCallbackIDs = [];
        };

        this.path = function()
        {
            return m_statePtr.path;
        };

        this.removeCB = function( cbid )
        {
            // first remove the CB from our own list (to make sure this is our callback)
            var ind = - 1;
            for( var i = 0 ; i < m_myCallbackIDs.length ; i ++ ) {
                if( m_myCallbackIDs[i] === cbid ) {
                    ind = i;
                    break;
                }
            }
            if( ind < 0 ) {
                throw new Error("no such callback found in private list");
            }
            else {
                m_myCallbackIDs.splice( ind, 1 );
            }
            // now remove the callback from the global list
            var callbacks = m_statePtr.callbacks;
            var ind = - 1;
            for( var i = 0 ; i < callbacks.length ; i ++ ) {
                if( callbacks[i].id === cbid ) {
                    ind = i;
                    break;
                }
            }
            if( ind < 0 ) {
                throw new Error("no such callback found in global list");
            }
            else {
                callbacks.splice( ind, 1 );
            }

            return m_that;
        };


        console.log( "current value:", m_statePtr.value );
    }

    connector.getSharedVar = function( path )
    {
        return new SharedVar( path );
    };

    connector.sendCommand = function( cmd, params, callback )
    {
        if( callback != null && typeof callback !== "function") {
            console.error( "What are you doing! I need a function for callback, not:", callback);
            throw new Error( "callback not a function in connector.sendCommand");
        }
        m_commandCallbacks.push( callback);
        QtConnector.jsSendCommandSlot( cmd, params);
    };

})();
/*

 (function( scope )
 {
 "use strict";

 return;

 var connector = mImport( "connector" );
 var console = mImport( "console" );
 connector.setConnectionCB( function( )
 {
 console.log( "connectionCB", connector, connector.getConnectionStatus() )
 } );
 connector.connect();

 return;

 scope.connector = {};

 scope.connector.setState = function( key, val )
 {
 pureweb.getFramework().getState().setValue( key, val );
 };

 scope.connector.clearState = function( prefix )
 {
 pureweb.getFramework().getState().getStateManager().deleteTree( prefix );
 };

 // rewrite uri if sharing session
 var uri = location.href;
 if( ! pureweb.getClient().isaSessionUri( uri ) ) {
 uri = location.protocol + '//' + location.host + '/pureweb/app?name=' + pureweb.getServiceAppName( uri );
 }

 var client = pureweb.getClient();
 pureweb.listen( client, pureweb.client.WebClient.EventType.CONNECTED_CHANGED, function onConnectedChanged( e )
 {
 if( ! e.target.isConnected() ) {
 return;
 }
 var diagnosticsPanel = document.getElementById( 'pwDiagnosticsPanel' );
 if( diagnosticsPanel ) {
 pureweb.client.diagnostics.initialize();
 }
 }
 );

 pureweb.listen( client, pureweb.client.WebClient.EventType.SESSION_STATE_CHANGED,
 function sscCB( e )
 {
 console.log( "pureweb session state changed", e );
 console.log( "  state:", client.getSessionState() );
 }
 );


 // setup the window.onbeforeunload callback to disconnect from the service application
 window.onbeforeunload = window.onunload = function( e )
 {
 if( client.isConnected() ) {
 client.disconnect( false );
 }
 return null;
 };

 pureweb.connect( uri );


 })( window );
 */

/*
 var img = Qt.img;
 if( imgEl == null) {
 imgEl = document.getElementById("mimg");
 }
 Qt.img.assignToHTMLImageElement( imgEl);

 */