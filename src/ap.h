#ifndef AP_H
#define API_H

typedef struct {
	char *ssid;
	char *psk;
} ap;

ap
newAP(const char *ssid, const char *psk)
{
	ap x;
	x.ssid = static_cast<char *>(ssid);
	x.psk = static_cast<char *>(psk);
	return x;
}

ap aps[] = {

};

#endif
