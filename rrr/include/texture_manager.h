/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelessa <cool.3meu@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/03 14:41:15 by dmelessa          #+#    #+#             */
/*   Updated: 2020/10/06 22:01:45 by dmelessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_MANAGER_H
# define TEXTURE_MANAGER_H

# include "rt_types.h"
# include "texture.h"
# include "perlin.h"
typedef struct s_texture_manager	t_texture_manager;

// struct material
// {
// 	type;
// 	texutre_id;
// };

struct	s_texture_manager
{
	t_texture	*textures;
	char		*imgs_data;
	cl_float4	*ranvec;
	int			*perm_x;
	int			*perm_y;
	int			*perm_z;
	size_t		textures_malloc_size;
	size_t		imgs_data_malloc_size;
	int			nimgs;
	int			ntextures;
	int			current_offset;
};

int		init_texture_manager(t_texture_manager *texture_manager);
int		add_texture(t_texture_manager *texture_manager, t_texture texture);
int		new_texture(t_texture_manager *texture_manager,
					t_texture_type type,
					void *data1,
					void *data2);

/**
** @brief
** set new texture for material and delete old
*/
// int		set_new_texture(t_te

#endif