---
name: JLUCS Speedrun
description: 温和、克制的就业指南阅读界面
colors:
  ground: "#faf9f5"
  paper: "#fffefa"
  ink: "#30302c"
  muted: "#75736b"
  line: "#e7e4db"
  accent: "#a75034"
  active: "#f0e6da"
  hover: "#f2f0e9"
  quote: "#f4f1e9"
  dark-ground: "#1b1b19"
  dark-paper: "#22221f"
  dark-ink: "#e6e0d7"
  dark-muted: "#b1aa9d"
  dark-line: "#3e3d36"
  dark-accent: "#e4ac88"
  dark-active: "#3a3028"
  dark-hover: "#2d2c26"
  dark-quote: "#292822"
typography:
  display:
    fontFamily: 'Georgia, "Songti SC", "SimSun", serif'
    fontSize: "clamp(32px, 3vw, 45px)"
    fontWeight: 600
    lineHeight: 1.3
    letterSpacing: "-0.025em"
  headline:
    fontFamily: '"Segoe UI", "PingFang SC", "Microsoft YaHei", sans-serif'
    fontSize: "29px"
    fontWeight: 600
    lineHeight: 1.5
    letterSpacing: "-0.02em"
  title:
    fontFamily: '"Segoe UI", "PingFang SC", "Microsoft YaHei", sans-serif'
    fontSize: "22px"
    fontWeight: 600
    lineHeight: 1.55
    letterSpacing: "-0.01em"
  body:
    fontFamily: '"Segoe UI", "PingFang SC", "Microsoft YaHei", sans-serif'
    fontSize: "16px"
    lineHeight: 1.95
  label:
    fontFamily: '"Segoe UI", "PingFang SC", "Microsoft YaHei", sans-serif'
    fontSize: "13px"
  code:
    fontFamily: "Consolas, monospace"
    fontSize: "0.86em"
rounded:
  inline: "3px"
  nav: "6px"
  control: "7px"
  field: "8px"
  panel: "9px"
  dialog: "12px"
spacing:
  paragraph: "20px"
  section: "24px"
  desktop-inset: "34px"
  desktop-gap: "42px"
components:
  icon-button:
    backgroundColor: "transparent"
    textColor: "{colors.ink}"
    rounded: "{rounded.control}"
    width: "36px"
    height: "36px"
  icon-button-hover:
    backgroundColor: "{colors.hover}"
  search-trigger:
    backgroundColor: "{colors.paper}"
    textColor: "{colors.muted}"
    rounded: "{rounded.field}"
    padding: "9px 11px"
    width: "232px"
  nav-active:
    backgroundColor: "{colors.active}"
    textColor: "{colors.accent}"
    rounded: "{rounded.nav}"
    padding: "9px 10px"
  quote-panel:
    backgroundColor: "{colors.quote}"
    textColor: "{colors.ink}"
    rounded: "{rounded.panel}"
    padding: "20px 23px"
  search-result:
    rounded: "{rounded.nav}"
    padding: "13px 12px"
---

# Design System: JLUCS Speedrun

## Overview

**Creative North Star: "就业指南阅读工具"**

采用用户指定的 csdiy.wiki 文档导航方式，以及 DiyJobResume 温和、克制的工具界面风格。暖白底、炭灰正文、陶土色链接和细分隔线支持长文阅读；标题使用衬线字，目录和操作使用系统无衬线字。

**Key Characteristics:**
- 正文优先，目录持续提供位置感。
- 暖色中性底与少量陶土强调色。
- 紧凑工具控件、清晰层级和宽松正文行距。
- 明暗主题共用同一组语义角色。

## Colors

### Primary

`accent` 用于正文链接、当前位置、焦点轮廓和阅读进度；`active` 承接选中项背景。深色模式使用对应的 `dark-` 色值。

### Neutral

`ground` 是页面底，`paper` 是搜索控件和对话框底；`ink` 是正文，`muted` 是辅助说明，`line` 是边界。`hover` 表示可交互的悬停区域，`quote` 用于引用及代码底。切换主题时全部角色一起切换，由 CSS 自定义属性提供实际颜色。

**The Semantic Theme Rule.** 新组件使用现有语义颜色变量；不要在组件内固定浅色主题颜色。

## Typography

正文与工具界面使用系统字体栈；主标题使用 Georgia、宋体回退，代码使用 Consolas、等宽回退。具体角色值以前置 tokens 为准。

一级标题采用自适应字号；章节入口若为二级标题，使用衬线字（34px，行高 1.45）。普通二、三级标题使用无衬线字，四级标题为（17px，650）。正文为（16px，行高 1.95），段后间隔使用 `paragraph`。导航为（13–14px），品牌为（19px），辅助说明为（12–13px）。

## Layout

桌面采用固定顶栏与三栏阅读网格：顶栏（76px），最大容器（1560px），章节目录（282px），弹性正文，本章目录（208px）；两侧内边距与栏间距使用前置 tokens。两个目录粘附于顶栏下，独立滚动。大于等于（1500px）时，正文列上限（760px），栏间距（56px）。

小于等于（1190px）隐藏本章目录，左目录缩为（250px），内边距（25px），栏间距（35px）。小于等于（780px）改为单列，顶栏（64px），正文两侧（22px）；章节目录成为宽度不超过（320px / 86vw）的抽屉。搜索入口只保留图标，次要顶栏链接隐藏。手机主标题（34px），入口二级标题（28px），普通二级标题（26px），三级标题（21px）。

图片保持原比例且不超出正文；表格和代码块允许横向滚动。打印隐藏导航与工具，展开所有章节。

## Elevation & Depth

阅读界面以边界和浅色背景划分层级，正文、目录与引用没有阴影。搜索对话框使用唯一浮层阴影（`0 20px 70px #0002`），背景遮罩（`#18171466`）；手机目录遮罩（`#0005`）。

## Shapes

小半径服务于不同密度：行内代码用 `inline`，导航与搜索结果用 `nav`，图标按钮用 `control`，搜索入口和代码块用 `field`，引用和品牌标记用 `panel`，对话框用 `dialog`。边界通常为（1px）细线；保留方正、紧凑的阅读工具轮廓。

## Components

- **工具按钮：** 透明底的方形图标按钮，悬停使用 `hover`；图标以线描 SVG 表达。
- **搜索：** 桌面为带快捷键提示的边框入口；对话框包含无边框输入、状态说明、结果和键盘提示。结果悬停或聚焦使用 `active`。支持 Ctrl/⌘ K、Enter 和 Esc。
- **章节导航：** 分组可折叠，当前章有暖色底；当前小节通过强调色文字和左边线标记。本章目录沿用左侧细线定位。
- **引用容器：** 低对比底色、细边框与适度圆角；手机内边距收为（16px 18px）。不将正文内容额外包装成卡片。
- **焦点：** 按钮、链接、输入与折叠摘要使用（2px）强调色轮廓及（4px）偏移；搜索输入在已有边界的搜索头内取消自身轮廓。提供跳到正文入口。
- **运动：** 页面锚点平滑滚动；手机抽屉以（0.2s，`cubic-bezier(.16,1,.3,1)`）移动。遵从减少动态效果偏好，取消平滑滚动、过渡和动画。

## Do's and Don'ts

### Do:
- **Do** 使用语义颜色变量，使组件随主题一起切换。
- **Do** 保留正文的宽松行距、清晰标题层级和持续可见的章节位置。
- **Do** 同时提供键盘焦点、手机抽屉和减少动态效果支持。

### Don't:
- **Don't** 添加渐变或装饰性主视觉。
- **Don't** 用重复卡片和常驻阴影包裹长文正文。
- **Don't** 让图片、代码或表格撑破手机阅读宽度。
