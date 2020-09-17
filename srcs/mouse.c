/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/20 10:51:39 by alex              #+#    #+#             */
/*   Updated: 2020/09/15 18:12:11 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "gui.h"


int		is_press_button(t_rt *rt, SDL_Rect *rect)
{
	if (rt->sdl.event.type == SDL_MOUSEBUTTONDOWN && rt->sdl.event.motion.y >= rect->y
		&& rt->sdl.event.motion.y <= rect->h
		&& rt->sdl.event.motion.x >= rect->x && rt->sdl.event.motion.x <= rect->w)
		return (1);
	else
		return (0);
}

void	is_pressed_checkbox(t_rt *rt, t_all_rect *all_rect, t_colors *color)
{

	if (rt->is_pressed.shadow == 1)
	{
		draw_ispressed_checkbox(rt, &all_rect->checkbox_button_shadow, "Shadow", color);
	}
	if (rt->is_pressed.ambient == 1)
	{
		draw_ispressed_checkbox(rt, &all_rect->checkbox_button_ambient, "Ambient", color);
	}
}

void	mouse_move(t_rt *rt, t_all_rect *all_rect, t_colors *color)
{
	if (is_press_button(rt, &all_rect->tab_main_button)) /* event tab main bar */
	{
		main_gui(rt, all_rect, color);
		draw_main_tab(rt, all_rect, color);
		color_tab_main(rt, color, all_rect);
		rt->is_pressed.main_tab = 1;
		rt->is_pressed.render_tab = 0;
	}
	if (is_press_button(rt, &all_rect->tab_render_button)) /* event tab render bar */
	{

		main_gui(rt,all_rect, color);
		draw_render_tab(rt, all_rect, color);
		color_tab_render(rt, color, all_rect);
		is_pressed_checkbox(rt, all_rect, color);
		rt->is_pressed.main_tab = 0;
		rt->is_pressed.render_tab = 1;
	}
	if (is_press_button(rt, &all_rect->checkbox_button_shadow)) /* shadow checkbox event */
	{
		main_gui(rt, all_rect, color);
		if (!rt->is_pressed.shadow)
		{
			draw_render_tab(rt, all_rect, color);
			color_tab_render(rt, color, all_rect);
			draw_ispressed_checkbox(rt, &all_rect->checkbox_button_shadow, "Shadow", color);
			rt->is_pressed.shadow = 1;
		}
		else
		{
			draw_render_tab(rt, all_rect, color);
			color_tab_render(rt, color, all_rect);
			// draw_checkbox(rt, &all_rect->checkbox_button_shadow, "Shadow", color);
			rt->is_pressed.shadow = 0;
		}
	}
	if (is_press_button(rt, &all_rect->checkbox_button_ambient))
	{
		main_gui(rt, all_rect, color);
		if (!rt->is_pressed.ambient)
		{
			draw_render_tab(rt, all_rect, color);
			color_tab_render(rt, color, all_rect);
			draw_ispressed_checkbox(rt, &all_rect->checkbox_button_ambient, "Ambient", color);
			rt->is_pressed.ambient = 1;
		}
		else
		{
			draw_render_tab(rt, all_rect, color);
			color_tab_render(rt, color, all_rect);
			// draw_checkbox(rt, &all_rect->checkbox_button_ambient, "Ambient", color);
			rt->is_pressed.ambient = 0;
		}
	}
	if (is_press_button(rt, &all_rect->fractol_button))
	{
		main_qjulia();
	}
}
