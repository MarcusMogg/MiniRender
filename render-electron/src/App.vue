<template>
  <bit-map />
</template>

<script>
import BitMap from "./components/Bitmap.vue";

export default {
  name: "App",
  components: {
    BitMap,
  },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
