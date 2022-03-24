#include "include/brkout.h"

bool RayVsRect(Vector2 ray_origin, Vector2 ray_dir, Rectangle target, Vector2 * contact_point, Vector2 * contact_normal, float * t_hit_near)
{
	/* All the functions dealing with collision were learned from Javid on the OneLoneCoder youtube channel.
	The functions were converted from C++ to C by Ali Asghar*/
	
	(*contact_point) = Vector2Zero();
	(*contact_normal) = Vector2Zero();
	
	//Find the inverse of the direction vector
	Vector2 inv = Vector2Divide(Vector2One(), ray_dir);
	
	//Calculate the vwector positions of the near and far intersections of the ray with the rectangle
	Vector2 t_near = (Vector2){(target.x - ray_origin.x) * inv.x, (target.y - ray_origin.y) * inv.y};
	Vector2 t_far = (Vector2){(target.x + target.width - ray_origin.x) * inv.x, (target.y + target.height - ray_origin.y) * inv.y};
	
	if (isnan(t_far.y) || isnan(t_far.x)) return false;
	if (isnan(t_near.y) || isnan(t_near.x)) return false;
	
	if(t_near.x > t_far.x) swap(&t_near.x, &t_far.x, sizeof(float));
	if(t_near.y > t_far.y) swap(&t_near.y, &t_far.y, sizeof(float));
	
	if(t_near.x > t_far.y || t_near.y > t_far.x) return false;
	
	*t_hit_near = U_MAX(t_near.x, t_near.y);
	float t_hit_far = U_MIN(t_far.x, t_far.y);
	
	if(t_hit_far < 0) return false;
	
	*contact_point = (Vector2){ray_origin.x + (ray_dir.x * (*t_hit_near)), ray_origin.y + (ray_dir.y * (*t_hit_near))};
	
	
	//Calculate the contact normal
	if(t_near.x > t_near.y)
	{
		if(inv.x < 0)
		{
			*contact_normal = (Vector2){1, 0};
		} else{
			*contact_normal = (Vector2){-1, 0};
		}
		
	} else if(t_near.x < t_near.y)
	{
		if(inv.y < 0)
		{
			*contact_normal = (Vector2){0, 1};
		} else{
			*contact_normal = (Vector2){0, -1};
		}
	}
	
	return true;
}
bool DynamicRectVsRect(Rectangle r_dynamic, Rectangle r_static, Vector2 velocity, Vector2 * contact_point, Vector2 * contact_normal, float * contact_time, float frame_time)
{
	//We assume the two rectangles arent already in collision
	if(velocity.x == 0 && velocity.y == 0) return false;
	
	Rectangle expanded = (Rectangle){r_static.x - r_dynamic.width / 2, r_static.y - r_dynamic.height / 2, r_static.width + r_dynamic.width, r_static.height + r_dynamic.height};
	Vector2 origin = (Vector2){r_dynamic.x + r_dynamic.width / 2, r_dynamic.y + r_dynamic.height / 2};
	Vector2 direction = (Vector2){velocity.x * frame_time, velocity.y * frame_time};
	
	if(RayVsRect(origin, direction, expanded, contact_point, contact_normal, contact_time))
	{
		return ((*contact_time) >= 0.0f && (*contact_time) <= 1.0f);
	}else
	{
		return false;
	}
	
}

void swap(void * a, void * b, size_t size)
{
	char temp[size];
  // char serves as the type for "generic" byte arrays

  memcpy(temp, b,    size);
  memcpy(b,    a,    size);
  memcpy(a,    temp, size);
}

int compare(const void *p1, const void *p2)
{
    const pair *elem1 = p1;    
    const pair *elem2 = p2;

   if (elem1->time < elem2->time)
      return -1;
   else if (elem1->time > elem2->time)
      return 1;
   else
      return 0;
}

int compareScores(const void *s1,const void *s2) 
{
    if (((players*)s1)->score > ((players*)s2)->score ) return -1;
    if (((players*)s1)->score == ((players*)s2)->score  ) return 0;
    return 1;
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



bool ResolveDynamicRectVsRect(ballstruct * r_dynamic, const float fTimeStep, blockstruct * r_static, float push_back)
{
	Vector2 contact_point, contact_normal;
	float contact_time = 0.0f;
	if (DynamicRectVsRect(r_dynamic->rect, r_static->rect, r_dynamic->velocity, &contact_point, &contact_normal, &contact_time, fTimeStep))
	{
		if (contact_normal.y > 0) r_dynamic->contact[0] = r_static->rect; else NULL;
		if (contact_normal.x < 0) r_dynamic->contact[1] = r_static->rect; else NULL;
		if (contact_normal.y < 0) r_dynamic->contact[2] = r_static->rect; else NULL;
		if (contact_normal.x > 0) r_dynamic->contact[3] = r_static->rect; else NULL;

		r_dynamic->velocity.x += contact_normal.x * (fabs(r_dynamic->velocity.x)) * (1 - contact_time) * push_back;
		r_dynamic->velocity.y += contact_normal.y * (fabs(r_dynamic->velocity.y)) * (1 - contact_time) * push_back;
		return true;
	}

	return false;
}


bool RectVsRectAABB(Rectangle a, Rectangle b)
{
	return (a.x < b.x + b.width && 
			a.x + a.width > b.x && 
			a.y < b.y + b.height && 
			a.y + a.height > b.y);
}

char *readfile(const char * dir, int * chars_in_line, int * total_num_lines)
{
	//Does not work for files greater than 4GB
	FILE *fptr;
	fptr = fopen(dir, "r");
	if(fptr)
	{
		findfileDim(fptr, chars_in_line, total_num_lines);
		//size_t file_size = ((*chars_in_line + 1)) * (*total_num_lines) * sizeof(char);
		char *buffer = (char*)calloc(((*chars_in_line + 1)) * (*total_num_lines), sizeof(char));
		if(buffer == NULL)
		{
			printf("Could not allocate the required memory!");
		} else
		{
			char c;
			int count = 0;
			for(c = getc(fptr); c != EOF; c = getc(fptr))
			{
				if(c != '\n' && c!= '\r') strncat(buffer, &c, 1);//buffer[count] = c;
				++count;
			}
			
			//fread(buffer, 1, file_size, fptr);
			buffer[count] = '\0';
		}
		fclose(fptr);
		return buffer;
	}
	fclose(fptr);
	printf("Could not find the file!");
	return NULL;
}

void findfileDim(FILE *fptr, int * chars_in_line, int *total_num_lines)
{
	char c;
	int length = 0, height = 0;
	if(fptr)
	{
		// Extract characters from file and store in character c
	    for (c = getc(fptr); c != EOF; c = getc(fptr))
	    {
	    	if (c == '\n') // Increment count if this character is newline
	    	{
	    		++height;
			} else if(height == 0)
			{
				++length;
			}
		}
		++height;
		//++length;
	}else{
		printf("Could not find the file!");
	}
	rewind(fptr);
	*chars_in_line = length;
	*total_num_lines = height;
}

players* GetSortedScores(const char * dir) {
	int i = 1, j = 0;
	char buffer[50];
	
	FILE *fptr ;
	
	if((fptr = fopen(dir, "r")) == NULL){
		printf("Score file not found. \n");
		return NULL;
	}
	fgets(buffer, 50, fptr);
	while(!feof(fptr)) 
	{
		fgets(buffer, 50, fptr);
		++i;
	}
	
	
	players *sc = (players *) malloc(i * sizeof(players));
	players *p = sc;
	
	fseek(fptr, 0, SEEK_SET);
	fgets(buffer, 50, fptr);
	
	while(!feof(fptr)) 
	{
		p = sc + j;
		sscanf(buffer,"%s %d", p->name, &p->score);
		fgets(buffer, 50, fptr);
		++j;
	}
	
	p = sc + j;
	sscanf(buffer, "%s %d", p->name, &p->score);
	
	fclose(fptr);
	
	qsort(sc, j + 1, sizeof(players), compareScores);
	
	return sc;
}

void addScore(const char * dir, char * name, int score) {
  
  FILE *fptr;
  if((fptr = fopen(dir, "a")) == NULL) printf("Could not find the score file.");
  fprintf_s(fptr, "\n%s %d", name, score);
  fclose(fptr);
}
int GenerateRandoms(int lower, int upper)
{
    
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

