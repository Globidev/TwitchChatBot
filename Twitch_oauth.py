import requests, urllib, urlparse
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
from webbrowser import open as openurl

LOCAL_HOST_ADDR = 'localhost'
LOCAL_HOST_PORT = 1337

RESPONSE_TYPE = 'code'
GRANT_TYPE = 'authorization_code'
REDIRECT_URI = 'http://%s:%s' % (LOCAL_HOST_ADDR, LOCAL_HOST_PORT)

SCOPE = 'chat_login'

AUTH_URL = 'https://api.twitch.tv/kraken/oauth2/authorize'
ACCES_URL = 'https://api.twitch.tv/kraken/oauth2/token'

authPayload = lambda clientId : {
    'response_type' : RESPONSE_TYPE,
    'client_id'     : clientId,
    'redirect_uri'  : REDIRECT_URI,
    'scope'         : SCOPE
}

accessPayload = lambda clientId, clientSecret : {
    'client_id'     : clientId,
    'client_secret' : clientSecret,
    'grant_type'    : GRANT_TYPE,
    'redirect_uri'  : REDIRECT_URI,
    'code'          : None
}

class RequestHandler(BaseHTTPRequestHandler) :

    def do_GET(s) :
        s.send_response(200)
        code = None
        try :
            code = urlparse.parse_qs(s.path[2:])['code']
        except KeyError : pass
        s.server.onCodeRetrieved(code)
        s.server.force_stop()

    def log_request(self, *args) :
        pass

class SingleRequestHTTPServer(HTTPServer) :

    def __init__(self, functor) :
        HTTPServer.__init__(self, (LOCAL_HOST_ADDR, LOCAL_HOST_PORT), RequestHandler)
        self.stopped = False
        self.onCodeRetrieved = functor

    def serve_forever(self) :
        while not self.stopped:
            self.handle_request()

    def force_stop(self):
        self.server_close()
        self.stopped = True

def getAccessToken(clientId, clientSecret) :
    authPayload_ = authPayload(clientId)
    accessPayload_ = accessPayload(clientId, clientSecret)

    def onCodeReceived(code) :
        accessPayload_['code'] = code
        
    server = SingleRequestHTTPServer(onCodeReceived)

    authUrl = '%s/?%s' % (AUTH_URL, urllib.urlencode(authPayload_))
    openurl(authUrl)

    try :
        server.serve_forever()
    except Exception, err :
        print err
        server.server_close()

    if accessPayload_['code'] is not None :
        response = requests.post(ACCES_URL, data = accessPayload_)
        json = response.json()
        return str(json['access_token'])
    return None