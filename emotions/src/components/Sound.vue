<template lang="pug">
  .sound(
    draggable
    :class="{ onDrag: onDrag, isPlaying: isPlaying }"
    @click="playSound"
    @dragstart="dragstart"
    @dragend="dragend"
    @dragenter="dragenter" )
    slot

</template>

<script>

const publicPath = process.env.BASE_URL;

const trimNumber = number => `000${number}`.slice(-4);

export default {
  name: 'Sound',
  data: () => ({
    onDrag: false,
    isPlaying: false,
  }),
  props: {
    mp3File: Number,
  },
  created() {
    const audioFile = `${publicPath}sounds/${trimNumber(this.mp3File)}.mp3`;
    // eslint-disable-next-line no-console
    console.log(audioFile);
    this.audio = new Audio(audioFile);
  },
  methods: {
    playSound() {
      this.isPlaying = true;
      this.audio.play().then(() => {
        setTimeout(() => {
          this.isPlaying = false;
         }, 500 )
      });
    },
    dragstart: function() {
      this.onDrag = true;
    },
    dragend: function() {
      this.onDrag = false;
    },
    dragenter: function() {
    }
  },
}
</script>

<style lang="sass">
.sound
  width: 50px
  height: 50px
  padding: 5px
  margin: 5px
  background: blue
  border-radius: 5px
  display: flex
  justify-content: center
  align-items: center
  color: white
  &.onDrag
    background-color: red
    opacity: .1
  &.isPlaying
    color: black
    background-color: yellow
</style>
