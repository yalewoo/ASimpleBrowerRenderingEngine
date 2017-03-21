###一个阉割版的html和css文件处理引擎。
绘图部分使用了cairo库。
该程序会读取html文件并生成结点树，读取css中的属性设置DOM树中的css属性。
目前支持的html标签有head link body div p h span strong em
支持的css属性有
display		inline	行元素
			block	块元素
			none	不显示。不输出
position	static	默认位置 忽略top bottom left right
			relative	相对定位（相对于在文档流中的位置）
			absolute	绝对定位（相对于不是static的父元素）
width height		auto	较为复杂的计算
					npx	n个px宽度
					n%	包含块宽度的n%
top bottom left right	position不是static	auto	较为复杂的计算
						npx	n个px宽度
						n%	包含块宽度的n%
padding		npx	0 0 0 0
			n%	相对于父元素的宽
border		npx	1px 1px 1px 1px
margin		npx	0 0 0 0
			n%	相对于父元素的宽
			nem	相当于与元素字体大小 字体大小*n
color		#RGB	#000
font-size		npx	16px
line-height		1.2	1.2倍的font-size
				1.2px	
text-align		left	
				center	
				right	
font-style		normal	
				italic	
font-weight		normal	
				bold	
line-break		normal	不自动换行
				break	自动换行