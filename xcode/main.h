// nathan@squimp.com do whatever you like with this
// expects byte type of 0-127 ranges for r,g,b gamma



// Not sure why the std libs of OSX do not include
// this struct def already.
typedef struct  {
	uint8_t     sin_len;
	sa_family_t sin_family;
	in_port_t   sin_port;
	struct  in_addr sin_addr;
	char    sin_zero[8];
} sockaddr_in;


/* I thought I would do this with floats
   but after looking at how Java handles
   sending floats, we will use bytes
typedef struct 
{
	float red;
	float green;
	float blue;
} gammaFractions;
*/

typedef struct 
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} gammaFractions;
