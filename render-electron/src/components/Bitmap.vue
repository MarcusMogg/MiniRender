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
    drawImage(buf) {
      const img = new ImageData(buf.Uint8ClampedArray, 800, 800);
      this.ctx?.putImageData(img, 0, 0);
    },

    handleFile(e) {
      console.log(e.target.files[0].path);
      console.log(ipcRenderer.hello());
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
