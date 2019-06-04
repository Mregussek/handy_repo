bool primeNumber(int n)
{
	if(n == 1)
		return false;
	else if(n == 2 || n == 3)
		return true;
	else if(n % 2 == 0 || n % 3 == 0)
		return false;
	else
	{
		int b = 5;
		
		while(b <= sqrt(n))
		{
			if(n % b == 0)
				return false;

			b += 2;

			if(n % b == 0)
				return false;
			
			b += 4;
		}

	return true;
}

3 5 7 9 11 13 15 17 19 ...
