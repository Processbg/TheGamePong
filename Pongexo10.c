#include "graphics.h"

#define N 100

void murs(POINT bg, int L)
{
	int H = L;
	POINT bd,hg,hd;
	
	bd.x = bg.x; bd.y = bg.y + L;
	hg.x = bd.x; hg.y = H; hd.x = hg.x + L; hd.y = hg.y;
	
	draw_line(bg,hg,vert);
	draw_line(bd,hd,rouge);
}

struct raquette
{
	POINT bg;
	POINT hd;
	
};

typedef struct raquette RAQUETTE;

RAQUETTE init_raquette()
{
	RAQUETTE r;
	
	r.bg.x = N/2;
	r.bg.y = N - 30;
	r.hd.x = N*2;
	r.hd.y = N/2;
	
	return r;
}

void affiche_raquette(RAQUETTE r)
{	
	draw_fill_rectangle(r.bg, r.hd, rouge);
}


void effacer_raquette(RAQUETTE r)
{
	draw_fill_rectangle(r.bg, r.hd, noir);
}

RAQUETTE deplacer_raquette(RAQUETTE r , int dx)
{	
	if((r.bg.x <= 4*N && r.bg.x >= 0) && (r.hd.x <= 4*N && r.hd.x >= N/2))
	{
		r.hd.x = r.hd.x + (10*dx);
		r.bg.x = r.bg.x + (10*dx);
	}
	
	if(((r.bg.x == 4*N) || (r.bg.x == 0)) || ((r.hd.x == 4*N) || (r.hd.x == N/2)))
	{
		r.hd.x = r.hd.x - (10*dx);
		r.bg.x = r.bg.x - (10*dx);	
	}
	
	return r;
}

struct balle
{
	POINT centre;
	COULEUR cool;
	int rayon;
	int dx;
	int dy;
};

typedef struct balle BALLE;

BALLE init_balle()
{
	BALLE b;
	
	b.centre.x = N;
	b.centre.y = N;
	b.cool = rouge;
	b.rayon = 20;
	b.dx = alea_int(10) - 5;
	b.dy = alea_int(2) + 5;
	
	return b;
}

void initialise_tab_balles(BALLE B[N])
{	
		B[0] = init_balle();
}

void affiche_balle(BALLE b, int visible)
{
	
	if (visible == 1)
		draw_fill_circle(b.centre, b.rayon, b.cool);
	else
		draw_fill_circle(b.centre, b.rayon, noir);
}

void affiche_des_balles(BALLE B[N], int  visible)
{
		affiche_balle(B[0], visible);
}

BALLE rebound(BALLE b, RAQUETTE r)
{
	if (b.centre.x - b.rayon < 0)   
		b.dx = -b.dx;
		
	if (b.centre.y - b.rayon < 0)   
		b.dy = -b.dy;
		
	if (((b.centre.x - b.rayon >= r.bg.x) && (b.centre.x + b.rayon <= r.hd.x))
		&& 
		((b.centre.y - b.rayon == r.hd.y) || (b.centre.y + b.rayon == r.hd.y))) 
		b.dy = -b.dy;	
		
	if (b.centre.x + b.rayon > 4*N) 
		b.dx = -b.dx;
		
	if (b.centre.y + b.rayon > 4*N) 
		b.dy = -b.dy;	
	
	return b;
}

 BALLE deplacer_balle(BALLE b, RAQUETTE r)
{	
	b.centre.y = b.centre.y + b.dy;
	b.centre.x = b.centre.x + b.dx;
	
	printf("(%d,%d) ray : %d %d %d\n",b.centre.x,b.centre.y,b.rayon,b.dx,b.dy);
	
	b = rebound(b,r);
	
	return b;
}

int choc (BALLE b, RAQUETTE r)
{
	int a,d;
	
	if (((b.centre.x - b.rayon >= r.bg.x) && (b.centre.x + b.rayon <= r.hd.x))
		&& 
		((b.centre.y - b.rayon == r.hd.y) || (b.centre.y + b.rayon == r.hd.y))) 
	{
		a = 1;
		return a;
	}	
	else
	{
		d = 0;
		return d;
	}		
}

int conter_vie(int vies, BALLE B, RAQUETTE R)
{
	int j;
	
	j = vies;
	
	if (B.centre.y - B.rayon < 0)
		j = vies-1;
				
	return j;
}

int conter_rebound()
{	
	return 0;
}

int main()
{
	int L = 4*N, H = 4*N;
	 
	init_graphics(L,H);
	
	POINT bg, p;
	p.x = 30; p.y = 4*N;
	bg.x = 0; bg.y = 0;
	murs(bg, L);
	
	RAQUETTE R = init_raquette();
	POINT P;
	int dx, vies = 3;
	BALLE B[N], b;
	
	initialise_tab_balles(B);
	affiche_des_balles(B,1);
	affiche_auto_off();
		
	while(vies != 0)
	{
		P = get_arrow();
		dx = P.x;
		if(dx != 0)	effacer_raquette(R);
		R = deplacer_raquette(R,dx);
		affiche_raquette(R);
		
		if(choc(B[0],R) == 1)
		{ 
			b = B[0];
			affiche_balle(B[0],0);
			B[0] = deplacer_balle(b,R);
		}
		affiche_balle(B[0],0);
		B[0] = deplacer_balle(B[0],R);
		affiche_balle(B[0],1); 
		
		murs(bg, L);
		attendre(10);
		vies = conter_vie(vies,B[0],R);
		
		aff_int(vies,20,p,blanc); 
		affiche_all();
	}
	
	
	wait_escape();
	exit(0);
}
