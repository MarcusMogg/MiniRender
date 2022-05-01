<template>
  <div>
    <input type="file" v-on:change="handleFile" />
    <canvas id="ccc" width="800" height="800" />
  </div>
</template>

<script>
const ipcRenderer = window.ipcRenderer;

export default {
  name: "BitMap",
  props: {},
  data() {
    return {
      ctx: null,
    };
  },

  methods: {
    drawImage() {
      console.log(ipcRenderer.vert());
      const buf = ipcRenderer.render();
      console.log(buf);
      console.log(ipcRenderer.vert());
      // const img = new ImageData(new Uint8ClampedArray(buf), 800, 800);

      let img = this.ctx.createImageData(800, 800);
      // const data = new Uint32Array(img.data.buffer);
      // const copy = new Uint8Array(800 * 800);
      // for (let index = 0; index < data.length; index++) {
      //   copy[index] = data[index];
      // }
      // console.log(img.data.buffer);
      img.data.set(buf);
      console.log(img);

      // for (let index = 0; index < data.length; index++) {
      //   if (copy[index] !== data[index]) {
      //     console.log(index);
      //     break;
      //   }
      // }
      this.ctx.scale(-1, 1);
      this.ctx.translate(0, 800);
      this.ctx.rotate(0, Math.PI);
      this.ctx.putImageData(img, 0, 0);
    },

    handleFile(e) {
      console.log(ipcRenderer.vert());
      console.log(ipcRenderer.loadFile(e.target.files[0].path));
      console.log(ipcRenderer.vert());
      this.drawImage();
      ipcRenderer.send("FileUrl", e.target.files[0].path);
    },
  },

  mounted() {
    const c = document.getElementById("ccc");
    this.ctx = c.getContext("2d");

    ipcRenderer.receive("Draw", (_, buf) => {
      this.drawImage(buf);
    });
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
