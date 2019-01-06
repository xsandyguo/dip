#ifndef DIP_RT_INDENT_H_
#define DIP_RT_INDENT_H_

#include <ostream>
#include "runtime/common.h"

namespace dip{
using namespace std;

class DIP_EXPORT FmtIndent {
public:
	void Delete() {delete this;};
	FmtIndent(int ind=0) {this->Indent=ind;};
	static FmtIndent *New();
	virtual const char *GetClassName() {return "DipIndent";};
	FmtIndent GetNextIndent();

	friend DIP_EXPORT ostream& operator<<(ostream& os, FmtIndent& o); 
protected:
	int Indent;  
};

} // namespace dip
#endif