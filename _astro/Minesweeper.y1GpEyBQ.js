import{w as u}from"./runtime-dom.esm-bundler.BMWejo6p.js";import{_ as g,M as x}from"./_plugin-vue_export-helper.RMgU4IPV.js";/* empty css                             */import{c as a,a as n,t as l,F as f,b as y,s as m,e as _,o as i,f as o,n as c}from"./runtime-core.esm-bundler.CqfEc3R6.js";const b={setup(){const t=m({gridWorld:null,board:null,gameover:!1,has_won:!1});return _(async()=>{try{const r=await x();t.gridWorld=new r.GridWorld(10,8),t.board=new r.Minesweeper}catch(r){console.error("Failed to load WebAssembly:",r)}}),{state:t}},computed:{getWidth(){return this.state.gridWorld?.width_readonly||0},getHeight(){return this.state.gridWorld?.height_readonly||0},locations(){if(!this.state.gridWorld)return[];const t=this.state.gridWorld.locations().keys();return Array.from({length:t.size()},(r,d)=>t.get(d))},getStatus(){return this.state.gameover?this.state.has_won?"😎":"😵":"🙂"}},methods:{reset(){this.state.board&&(this.state.board.reset(),this.state.gameover=!1,this.state.has_won=!1,this.$forceUpdate())},flag(t){!this.state.board||this.state.gameover||this.state.board.isExplored(t)||(this.state.board.toggleFlag(t),this.state.board.checkWin()&&(this.state.has_won=!0,this.state.gameover=!0),this.$forceUpdate())},explore(t){!this.state.board||this.state.gameover||(this.state.board.explore(t),this.state.board.isMine(t)?this.state.gameover=!0:this.state.board.checkWin()&&(this.state.has_won=!0,this.state.gameover=!0),this.$forceUpdate())},getMineCount(t){return this.state.board?.getMineCount(t)||""},isMine(t){return this.state.board?.isMine(t)||!1},isExplored(t){return this.state.board?.isExplored(t)||!1},isFlag(t){return this.state.board?.isFlag(t)||!1},classFor(t){return this.state.board?.isExplored(t)?"explored":""}}},w={id:"diagram1"},p={class:"d-flex flex-column align-items-center"},v=["viewBox"],k=["onClick","onContextmenu"],M=["x","y"],W=["x","y"],C=["cx","cy"],F={key:2,stroke:"rgb(51, 151, 217)","stroke-width":"0.1"},E=["x1","y1","x2","y2"],B=["x1","y1","x2","y2"];function z(t,r,d,S,U,s){return i(),a("figure",w,[n("div",p,[n("button",{type:"button",class:"btn btn-primary btn-lg btn-block mb-4",onClick:r[0]||(r[0]=(...e)=>s.reset&&s.reset(...e))},l(s.getStatus),1),(i(),a("svg",{viewBox:`0 0 ${s.getWidth} ${s.getHeight}`,onContextmenu:r[1]||(r[1]=u(()=>{},["prevent"]))},[(i(!0),a(f,null,y(s.locations,e=>(i(),a("g",{onClick:h=>s.explore(e),onContextmenu:h=>s.flag(e)},[n("rect",{class:c("cell "+s.classFor(e)),x:e.x,y:e.y,width:"1",height:"1"},null,10,M),s.isExplored(e)&&!s.isMine(e)?(i(),a("text",{key:0,"text-anchor":"middle","font-size":.45,x:e.x+.5,y:e.y+.5,dy:.15},l(s.getMineCount(e)),9,W)):o("",!0),s.isExplored(e)&&s.isMine(e)?(i(),a("circle",{key:1,cx:e.x+.5,cy:e.y+.5,r:"0.25",fill:"rgb(229, 74, 58)"},null,8,C)):o("",!0),s.isFlag(e)?(i(),a("g",F,[n("line",{x1:e.x+.2,y1:e.y+.2,x2:e.x+.8,y2:e.y+.8},null,8,E),n("line",{x1:e.x+.2,y1:e.y+.8,x2:e.x+.8,y2:e.y+.2},null,8,B)])):o("",!0)],40,k))),256))],40,v))])])}const D=g(b,[["render",z],["__scopeId","data-v-9a0f8b12"]]);export{D as default};
