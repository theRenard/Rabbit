<template lang="pug">
  .sound(
    draggable
    :class="{ isPlaying: isPlaying }"
    @click.stop="playSound"
    @dragstart="dragstart"
    @dragend="dragend" )
    .remove( @click.stop="$emit('destroy')" ) -
    span {{ mp3File }}

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
    dragstart: function(event) {
      this.onDrag = true;
      event.dataTransfer.setData("fileNumber", this.mp3File);
    },
    dragend: function() {
      this.onDrag = false;
    },
  },
}
</script>

<style lang="scss">
.sound {
  position: relative;
  width: 40px;
  height: 40px;
  padding: 5px;
  margin: 7px 0 0 7px;
  border-radius: 5px;
  border: 2px solid black;
  display: flex;
  flex: 0 0 auto;
  justify-content: center;
  align-items: center;
  color: black;
  cursor: move;
  cursor: grab;
  font-weight: bold;
  &:active {
    cursor: grabbing;
    background-color: red;
    opacity: .1;
  }
  &.isPlaying {
    color: black;
    background-color: yellow;
  }
  .remove {
    position: absolute;
    right: -5px;
    top: -5px;
    width: 15px;
    height: 15px;
    background-color: red;
    border-radius: 50%;
    font-size: 25px;
    color: white;
    line-height: 12px;
    text-align: center;
    display: none;
    cursor: pointer;
  }
  &.selected {
    &:hover {
      .remove {
        display: block;
      }
    }
  }
}
</style>
