package com.theo.nexplayer.dns;

import java.util.Collections;
import java.util.Map;

/**
 * thread safe lru cache
 *
 * @author txy
 */
public class SyncLruCache<K, V> {

    private Map<K, V> mSyncMap;
    private int mMaxSize;

    /**
     * fixed size cache
     *
     * @param size
     */
    public SyncLruCache(int size) {
        mSyncMap = Collections.synchronizedMap(new LruLinkedHashMap(size));
        mMaxSize = size;
    }

    public V cache(K key, V value) {
        return mSyncMap.put(key, value);
    }

    public V get(K key) {
        return mSyncMap.get(key);
    }

    public int size() {
        return mSyncMap.size();
    }

    public void clear() {
        mSyncMap.clear();
    }

}
