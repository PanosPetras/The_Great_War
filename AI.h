#ifndef AI_H
#define AI_H

class Request;

class AI {
public:

protected:
	static void HandleRequest(Request& req);

	friend class Country;
};

#endif
