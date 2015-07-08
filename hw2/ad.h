class Adproperty{
public:
	int click;
	int impression;
	unsigned long long int url;
	int adid;
	int advertiserid;
	int depth;
	int position;
	int queryid;
	int keywordid;
	int titleid;
	int descriptionid;
};

class PartAd{
public:
	unsigned long long int url;
	int advertiserid;
	int keywordid;
	int titleid;
	int descriptionid;
	bool operator<(const PartAd& R) const{
		if(url < R.url)
			return true;
		else if(url == R.url)
			if(advertiserid < R.advertiserid)
				return true;
			else if(advertiserid == R.advertiserid)
				if(keywordid < R.keywordid)
					return true;
				else if(keywordid == R.keywordid)
					if(titleid < R.titleid)
						return true;
					else if(titleid == R.titleid)
						if(descriptionid < R.descriptionid)
							return true;
		return false;
	}
};

class Cl{
public:
	int userid;
	int click;
	int impression;
};