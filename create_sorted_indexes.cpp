/// ----------------------------------------------------------------------------------------------------------- create_sorted_indexes.cpp

// example:
// input:  30, 10, 20, 50, 0
// output:  4,  1,  2,  0, 3   --> meaning: the smallest is at index 4    (value: 0)
//                                          second smallest is at index 1 (value: 10)
//                                          third smallest is at index 2  (value: 20)

template<class T, class P>
vector<int> create_sorted_indexes(const T &container, P Pred)
{
	vector<int> indexes(container.size());
	iota(ALL(indexes), 0);
	sort(indexes.begin(), indexes.end(), [&container, Pred](int a, int b) { return Pred(container[a], container[b]); });
	return indexes;
}

// example:

// create_sorted_indexes          input:   30, 10, 20, 50, 0       ---  this would be sorted 0, 10, 20, 30, 50 
//                               output:    4,  1,  2,  0, 3    (meaning above)
// create_old_index_to_new_index  input:    4,  1,  2,  0, 3   
// create_old_index_to_new_index output:    3,  1,  2,  4, 0   --> meaning: 30 would be at (zero based) index 3 if original input was sorted
//                                                                          10 would be at index 1 if original input was sorted

vector<int> create_old_index_to_new_index(const vector<int> &new_indexes)
{
	vector<int> result(new_indexes.size());
	for(int i=0, iend=(int)new_indexes.size(); i!=iend; ++i)
		result[new_indexes[i]]=i;
	return result;
}

/// ----------------------------------------------------------------------------------------------------------- end of create_sorted_indexes.cpp
