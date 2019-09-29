template<int rows, int coloumns>
class tMatrix
{
	array<array<ll, coloumns>, rows> mCells;
public:
	constexpr int Rows() const { return rows; }
	constexpr int Coloumns() const { return coloumns; }
	ll GetCell(int row, int coloumn) const { return mCells[row][coloumn]; }
	ll &AccessCell(int row, int coloumn) { return mCells[row][coloumn]; }
	tMatrix<rows, coloumns> operator+(const tMatrix<rows, coloumns> &Other);
	tMatrix<rows, coloumns> operator-(const tMatrix<rows, coloumns> &Other);
	template<int other_coloumns> tMatrix<rows, other_coloumns> operator*(const tMatrix<rows, other_coloumns> &Other);
};

template<int rows, int coloumns>
tMatrix<rows, coloumns> tMatrix<rows, coloumns>::operator+(const tMatrix<rows, coloumns> &Other)
{
	tMatrix<rows, coloumns> result;
	for(int r=0; r<rows; ++r)
		for(int c=0; c<coloumns; ++c)
			result.AccessCell(r, c)=GetCell(r, c)+Other.GetCell(r, c);
	return result;
}

template<int rows, int coloumns>
tMatrix<rows, coloumns> tMatrix<rows, coloumns>::operator-(const tMatrix<rows, coloumns> &Other)
{
	tMatrix<rows, coloumns> result;
	for(int r=0; r<rows; ++r)
		for(int c=0; c<coloumns; ++c)
			result.AccessCell(r, c)=GetCell(r, c)-Other.GetCell(r, c);
	return result;
}

template<int rows, int coloumns>
template<int other_coloumns> tMatrix<rows, other_coloumns> tMatrix<rows, coloumns>::operator*(const tMatrix<rows, other_coloumns> &Other)
{
	tMatrix<rows, other_coloumns> result;
	for(int tr=0; tr<result.Rows(); ++tr)
	{
		for(int tc=0; tc<result.Coloumns(); ++tc)
		{
			ll c=0;
			for(int i=0; i<Coloumns(); ++i)
				c+=GetCell(tr, i)*Other.GetCell(i, tc);
			result.AccessCell(tr, tc)=c;
		}
	}
	return result;
}


template<class M> 
void PrintMatrix(const M &Matrix)
{
	for(int r=0; r<Matrix.Rows(); ++r)
		for(int c=0; c<Matrix.Coloumns(); ++c)
			printf("%lld\n", Matrix.GetCell(r, c));
}

template<class M>
void ReadMatrix(M &Matrix)
{
	for(int r=0; r<Matrix.Rows(); ++r)
		for(int c=0; c<Matrix.Coloumns(); ++c)
			scanf("%lld", &Matrix.AccessCell(r, c));
}
