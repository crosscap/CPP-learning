#include <string>

using std::string;

class Token
{
public:
	Token()
		: tok(INT), ival{0} { }
	Token(const Token &t)
		: tok(t.tok) { copyUnion(t); }
	Token &operator=(const Token &);
	~Token();

	Token &operator=(const std::string &);
	Token &operator=(char);
	Token &operator=(int);
	Token &operator=(double);

private:
	enum { INT,
		   CHAR,
		   DBL,
		   STR } tok;
	union {
		char cval;
		int ival;
		double dval;
		std::string sval;
	};
	void copyUnion(const Token &);
};

Token &Token::operator=(const Token &t)
{
	if (tok == STR && t.tok != STR)
		sval.~string();
	if (tok == STR && t.tok == STR)
		sval = t.sval;
	else
		copyUnion(t);
	tok = t.tok;
	return *this;
}

Token::~Token()
{
	if (tok == STR)
		sval.~string();
}

Token &Token::operator=(const std::string &s)
{
	if (tok == STR)
		sval = s;
	else
		new (&sval) string(s);
	tok = STR;
	return *this;
}

Token &Token::operator=(char c)
{
	if (tok == STR)
		sval.~string();
	cval = c;
	tok = CHAR;
	return *this;
}

Token &Token::operator=(int i)
{
	if (tok == STR)
		sval.~string();
	ival = i;
	tok = INT;
	return *this;
}

Token &Token::operator=(double d)
{
	if (tok == STR)
		sval.~string();
	dval = d;
	tok = DBL;
	return *this;
}

void Token::copyUnion(const Token &t)
{
	switch (t.tok) {
	case INT:
		ival = t.ival;
		break;
	case CHAR:
		cval = t.cval;
		break;
	case DBL:
		dval = t.dval;
		break;
	case STR:
		new (&sval) string(t.sval);
		break;
	}
}