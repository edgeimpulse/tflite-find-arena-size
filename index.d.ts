declare module "tflite-find-arena-size" {
    class FindArenaSize {
        init(): Promise<void>;
        findArenaSize(buffer: Buffer, low: number, high: number): number;
    }

    export = FindArenaSize;
}
