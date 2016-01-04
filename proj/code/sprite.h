#ifndef __SPRITE_H
#define __SPRITE_H


/** @defgroup sprite sprite
 * @{
 *
 * Functions related to the objects to be displayed on screen.
 */


/** @name Sprite */
/**@{
 *
 * Object to be displayed on screen.
 */

typedef struct {
	int x, y; /*!< Coordinates of the sprite. */
	int width /*!< Dimensions of the sprite. */ , height; /*!< Dimensions of the sprite. */
	int x_speed /*!< Speed of the sprite. */ , y_speed; /*!< Speed of the sprite. */
	int jumping; /*!< Used to track if the sprite is jumping (1) or not (0). */
	int inplatform; /*!< Used to track if the sprite is on a platform (1) or not (0). */
	char *map; /*!< Pointer to the pixmap of the sprite. */
} Sprite;


/*! \brief Creates a sprite.
*
*	Creates a sprite using the pixmap and coordinates given. The rest of the parameters are initialized to 0.
*
*	\param pic pointer to pixmap.
*	\param x coordinate for the sprite.
*	\param y coordinate for the sprite
*	\return pointer to the created sprite.
*/
Sprite *create_sprite(char *pic[], unsigned short x, unsigned short y);

/*! \brief Draws a sprite.
*
*	Draws a sprite in the buffer given.
*
*	\param sp pointer to a sprite.
*	\param video_mem pointer to a buffer.
*	\return 0 if the sprites fits in the buffer. 1 if not.
*/
int draw_sprite(Sprite *sp, char *video_mem);

/*! \brief Moves a sprite.
*
*	Moves a sprite, checking the collisions with the borders and other sprites. Updates its position, jumping and inplatform.
*
*	\param sp pointer to a sprite.
*	\param h_res width of the buffer.
*	\param v_res height of the buffer.
*	\param video_mem pointer to a buffer.
*	\return 1-4 if the sprite collides with a border. 5-7 if it collides with another sprite. 0 if there are no collisions.
*/
int move_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem);


/*! \brief Moves a shot sprite.
*
*	Moves a shot sprite, checking the collisions with the borders and other sprites. Updates its position.
*	Also checks if it hits the other player.
*
*	\param sp pointer to the shot sprite.
*	\param p pointer to the player to hit sprite.
*	\param h_res width of the buffer.
*	\param v_res height of the buffer.
*	\param video_mem pointer to a buffer.
*	\return 1-4 if the sprite collides with a border. 5 if it collides with a sprite other that the player's. 6 if it collides with the player. 0 if there are no collisions.
*/
int move_shot(Sprite *sp, Sprite *p, unsigned short h_res, unsigned short v_res, char *video_mem);

/*! \brief Sets a sprite's horizontal speed to the specified.
*
*	\param sp pointer to a sprite.
*	\param x_speed sprite's new x speed.
*/
void set_x_speed_sprite(Sprite *sp, int x_speed);

/*! \brief Sets a sprite's vertical speed to the specified.
*
*	\param sp pointer to a sprite.
*	\param y_speed sprite's new y speed.
*/
void set_y_speed_sprite(Sprite *sp, int y_speed);

/*! \brief Frees the memory occupied by the sprite.
*
*	\param sp pointer to a sprite.
*/
void destroy_sprite(Sprite *sp);


#endif
