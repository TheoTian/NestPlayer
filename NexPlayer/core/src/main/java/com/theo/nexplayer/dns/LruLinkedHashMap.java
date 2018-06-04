package com.theo.nexplayer.dns;

import java.util.LinkedHashMap;

/**
 * if size bigger than max size
 * remove the eldest
 * <p>
 * <p>
 * init:
 * head<->head
 * <p>
 * add:
 * head<->x<->head
 * head<->x<->y<->head
 * <p>
 * eldest = head.after
 * newest = head.before
 *
 * @author txy
 */
public class LruLinkedHashMap extends LinkedHashMap {

    private final static int DEFAULT_MAX_SIZE = 128;
    private int mMaxSize = DEFAULT_MAX_SIZE;

    public LruLinkedHashMap(int maxSize) {
        if (maxSize > 0) {
            mMaxSize = maxSize;
        }
    }

    @Override
    protected boolean removeEldestEntry(Entry eldest) {
        return size() > mMaxSize;
    }
}
