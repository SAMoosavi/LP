#ifndef LP_SIMPLEX_H
#define LP_SIMPLEX_H

#include <bits/stdc++.h>
#include "LP.h"
#include "InputLP.h"
#include "ColoredString.h"

using namespace std;

class Simplex {
public:
	typedef double ValueOfX;
	typedef vector<ValueOfX> ListOfX;
	typedef function<ValueOfX(const ListOfX &)> Transformer;
	typedef vector<Transformer> Transformers;

	Simplex(LP last_lp);

private:
	void creat_std_lp(const LP & last_lp);

	LP lp;
	Transformers transformers;
	size_t number_of_s;
};

#endif
